/*------------------------------------------------------------------------------
 * File: graphics.c
 * Created: June 8, 2015
 * Last changed: June 16, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder ett enkelt grafikbibliotek för att rita och visa grafik. Biblio-
 *   teket använder OpenGL och renderar således med hårdvara om möjlighet finns.
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "graphics.h"

#include "core/array.h"
#include "core/common.h"
#include "core/linmath.h"

#include <stdlib.h>

#include <GL/glew.h>

#include <Windows.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: ClassName
 *
 * Description:
 *   The name of the class used to create the graphics window.
 *------------------------------------*/
#define ClassName (L"sa14-game1")

/*--------------------------------------
 * Constant: DefaultFPS
 *
 * Description:
 *   The number of frames to display each second, by defaukt. This can be
 *   configured with the setFrameRate() function.
 *------------------------------------*/
#define DefaultFPS (30.0f)

/*--------------------------------------
 * Constant: UnlimitedFPS
 *
 * Description:
 *   Unlimited number of frames per second.
 *------------------------------------*/
#define UnlimitedFPS (-1)

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: triMeshT_
 *
 * Description:
 *   Represents a piece of geometry with a vertex mesh etc. This type also
 *   contains the private section of the type, which is inaccessible outside
 *   this module. The order of the fields must not be changed to preserve
 *   compatibility with the public part of this type.
 *------------------------------------*/
typedef struct {
    /* --- Public --- */

    vertexT *verts;     /* The vertices.           */
    int      num_verts; /* Number of vertices.     */
    triT    *tris;      /* The triangles (faces).  */
    int      num_tris;  /* Number of triangles.    */

    /* --- Private --- */

    GLuint vbo, /* Vertex buffer. */
           ibo; /* Index buffer.  */
} triMeshT_;

/*--------------------------------------
 * Type: shaderProgramCDT
 *
 * Description:
 *   Abstrakt pekartyp för ett shader-program.
 *------------------------------------*/
struct shaderProgramCDT {
    GLuint  id;      /* Shader-programmets interna ID-nummer. */
    arrayT *shaders; /* Pekare till array med programmets shaders. */
};

/*--------------------------------------
 * Type: windowT
 *
 * Description:
 *   Datatyp som representerar ett fönster.
 *------------------------------------*/
typedef struct {
    const char* title;      /* Fönstrets titel.                      */
    int         width,      /* Fönstrets bredd i antal pixlar.       */
                height;     /* Fönstrets höjd i antal pixlar.        */
    int         frame_time; /* Den tid som varje bildruta ska visas. */

    /* Nedan är plattformsspecifika, systemrelaterade variabler. */

    HWND          hwnd;        /* Systemets egna "handtag" till fönstret.    */
    HDC           hdc;         /* Den DC (device context) som används.       */
    HGLRC         hglrc;       /* Den renderingskontext som används.         */
    LARGE_INTEGER last_update; /* Tidsstämpel för senaste bilduppdateringen. */
} windowT;

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

/*--------------------------------------
 * Variable: window
 *
 * Description:
 *   Pekare till grafikfönstret.
 *------------------------------------*/
static windowT *window = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*------------------------------------------------------------------------------
 * Nedan är de privata funktionerna som inte är tänkta att användas utanför
 * denna modul.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: WindowProc()
 *
 * Description:
 *   http://en.wikipedia.org/wiki/WindowProc
 *------------------------------------*/
static LRESULT CALLBACK WindowProc(_In_ HWND   hwnd,
                                   _In_ UINT   uMsg,
                                   _In_ WPARAM wParam,
                                   _In_ LPARAM lParam)
{
    switch (uMsg) {

    case WM_CLOSE: {
        /* The user has closed the window, so we exit graphics mode. */
        exitGraphics();
        break;
    }

    };

    return (DefWindowProc(hwnd, uMsg, wParam, lParam));
}

/*--------------------------------------
 * Function: checkGraphicsInited()
 * Parameters:
 *
 * Description:
 *   Invokes the error() macro if graphics mode has not been initialized.
 *------------------------------------*/
static void checkGraphicsInited(void) {
    if (!window)
        error("Graphics not initialized");
}

/*--------------------------------------
 * Function: compileShader()
 * Parameters:
 *   type    Den typ av shader som ska kompileras.
 *   prog    Shader-programmet som shadern ska kompileras in i och länkas till.
 *   source  Shader-programmets GLSL-kod..
 *
 * Description:
 *   Kompilerar en shader och länkar den till det specificerade shader-
 *   programmet.
 *------------------------------------*/
static void compileShader(GLenum type, shaderProgramADT prog, string source) {
    GLuint shader_id = glCreateShader(type);

    glShaderSource (shader_id, 1, &source, NULL);
    glCompileShader(shader_id);

    GLint result;

    /*
     * Om GL_COMPILE_STATUS returnerar GL_FALSE i result-parametern så miss-
     * lyckades kompileringen, förmodligen på grund av trasig kod. Vi kan inte
     * fortsätta då utan genererar ett fel istället, samt skriver ut vad OpenGL
     * innehåller för loggmeddelanden kring kompileringen.
     */
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLint info_log_length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        GLchar *log = malloc(sizeof(GLchar) * info_log_length);
        glGetShaderInfoLog(shader_id, info_log_length, NULL, log);
        printf("\n%s", log);
        free(log);
        error("Shader failed to compile");
    }

    glAttachShader(prog->id, shader_id);
    glLinkProgram (prog->id);

    glGetProgramiv(prog->id, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        GLint info_log_length;
        glGetProgramiv(prog->id, GL_INFO_LOG_LENGTH, &info_log_length);
        GLchar *log = malloc(sizeof(GLchar) * info_log_length);
        glGetProgramInfoLog(prog->id, info_log_length, NULL, log);
        printf("\n%s", log);
        free(log);
        error("Shader program failed to link");
    }

    arrayAdd(prog->shaders, &shader_id);
}

/*--------------------------------------
 * Function: createWindow()
 * Parameters:
 *   title   Fönstrets titel.
 *   width   Fönstrets bredd, i antal pixlar.
 *   height  Fönstrets höjd, i antal pixlar.
 *
 * Description:
 *   Skapar ett grafikfönster med de specificerade dimensionerna. Den angivna
 *   bredden och höjden inkluderar inte fönsterdekorationer, utan endast
 *   storleken på klientytan.
 *------------------------------------*/
static void createWindow(string title, int width, int height) {
    RECT  rect  = { 0 }; rect.right = width; rect.bottom = height;
    DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

    /*
     * Här justerar vi storleken på fönstret så att de angivna dimensionerna
     * inte inkluderar fönsterdekorationer så som kantområden, titelområde m.m.
     */
    assert(AdjustWindowRectEx(&rect, style, FALSE, WS_EX_LEFT));

    /* Dags att allokera fönsterdatatypen. */
    window = malloc(sizeof(windowT));

    /*
     * Eftersom CreateWindowExW()-funktionen vill ha Unicode-strängar, så måste
     * vi konvertera titel-strängen till en Unicode-sträng och skicka den till
     * funktionen.
     */
    size_t   window_name_length = mbstowcs(NULL, title, 0) + 1;
    wchar_t *window_name        = malloc(sizeof(wchar_t) * window_name_length);

    mbstowcs(window_name, title, window_name_length);

    /* Här skapar vi fönstret med ett anrop ner i Windows API. */
    window->hwnd = CreateWindowExW(WS_EX_LEFT,
                                   ClassName,
                                   window_name,
                                   style,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   rect.right - rect.left,
                                   rect.bottom - rect.top,
                                   HWND_DESKTOP,
                                   NULL,
                                   GetModuleHandleW(NULL),
                                   NULL);

    /*
     * CreateWindowEx()-funktionen kopierar titel-strängen så det är ok att
     * deallokera den här.
     */
    free(window_name);

    /* Om window->hwnd är NULL så skapades inget fönster. */
    assert(window->hwnd != NULL);

    /* Utan detta anrop syns inte fönstret. */
    ShowWindow(window->hwnd, SW_SHOW);

    window->width  = width;
    window->height = height;
    window->title  = title;

    window->hdc = GetDC(window->hwnd);
    
    assert(window->hdc != NULL);
}

/*--------------------------------------
 * Function: registerWindowClass()
 *
 * Description:
 *   Registrerar fönsterklassen.
 *------------------------------------*/
static void registerWindowClass(void) {
    /*
     * Innan vi kan skapa fönster kräver Windows att vi registrerar en fönster-
     * klass.
     */

    WNDCLASSEXW wcx = { 0 };

    wcx.cbSize        = sizeof(WNDCLASSEXW);
    wcx.style         = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc   = WindowProc;
    wcx.cbClsExtra    = 0;
    wcx.cbWndExtra    = 0;
    wcx.hInstance     = GetModuleHandleW(NULL);
    wcx.hIcon         = LoadIconW(NULL, IDI_APPLICATION);
    wcx.hCursor       = LoadCursorW(NULL, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcx.lpszMenuName  = NULL;
    wcx.lpszClassName = ClassName;
    wcx.hIconSm       = NULL;

    assert(RegisterClassExW(&wcx));
}

/*--------------------------------------
 * Function: setupPixelFormat()
 * Parameters:
 *
 * Description:
 *   Initierar grafikläge för fönstret.
 *------------------------------------*/
static void setupPixelFormat(void) {
    /* Detta krävs för att ett fönster ska acceptera OpenGL-läge. */

    PIXELFORMATDESCRIPTOR pfd = { 0 };

    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixel_format = ChoosePixelFormat(window->hdc, &pfd);

    assert(pixel_format != 0);
    assert(SetPixelFormat(window->hdc, pixel_format, &pfd));
}

/*--------------------------------------
 * Function: unregisterWindowClass()
 *
 * Description:
 *   Avregistrerar fönsterklassen.
 *------------------------------------*/
static void unregisterWindowClass(void) {
    assert(UnregisterClassW(ClassName, GetModuleHandleW(NULL)));
}

/*------------------------------------------------------------------------------
 * Här nedanför är de publika funktionerna som är tänkta att användas av klient-
 * programmen för att initiera grafikläget m.m.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Funktioner för att initiera grafikläge, avsluta grafikläge samt konfigurera
 * grafikläget och ställa in parametrar.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: initGraphics()
 * Parameters:
 *   title   Fönstrets titel.
 *   width   Fönstrets bredd, i antal pixlar.
 *   height  Fönstrets höjd, i antal pixlar.
 *
 * Description:
 *   Skapar ett grafikfönster med de specificerade dimensionerna. Den angivna
 *   bredden och höjden inkluderar inte fönsterdekorationer, utan endast
 *   storleken på klientytan.
 *------------------------------------*/
void initGraphics(string title, int width, int height) {
    if (window)
        error("Graphics already initialized");

    registerWindowClass();
    createWindow(title, width, height);
    setupPixelFormat();

    window->hglrc = wglCreateContext(window->hdc);

    assert(window->hglrc != NULL);
    assert(wglMakeCurrent(window->hdc, window->hglrc));
    assert(glewInit() == GLEW_OK);

    /* Enabling GL_BLEND makes transparency possible. */
    glEnable   (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Ignore triangles that are "looking away" from the camera. */
    glEnable(GL_CULL_FACE);

    /*
     * Enable z-buffering and make sure it's right-handed (i.e. -z is into the
     * screen).
     */
    glEnable(GL_DEPTH_TEST);
    glClearDepth(0.0f);
    glDepthFunc(GL_GREATER);

    setFrameRate(DefaultFPS);
}

/*--------------------------------------
 * Function: exitGraphics()
 * Parameters:
 *
 * Description:
 *   Stänger grafikfönstret.
 *------------------------------------*/
void exitGraphics(void) {
    /* Om window redan är NULL så är vi inte i grafikläge. */
    if (!window)
        return;

    assert(wglMakeCurrent(window->hdc, NULL));
    assert(wglDeleteContext(window->hglrc));

    window->hdc   = NULL;
    window->hglrc = NULL;

    assert(DestroyWindow(window->hwnd));

    /* @To-do: Är det en bra idé att anropa updateWindow() här? */
    updateDisplay();

    /* Vi städar upp efter oss genom att avregistrera fönsterklassen här. */
    unregisterWindowClass();

    free(window);
    window = NULL;
}

/*--------------------------------------
 * Function: createShaderProgram()
 * Parameters:
 *
 * Returns:
 *   En pekare till ett shader-program.
 *
 * Description:
 *   Skapar ett nytt shader-program.
 *------------------------------------*/
shaderProgramADT createShaderProgram(void) {
    shaderProgramADT program = malloc(sizeof(struct shaderProgramCDT));

    program->id = glCreateProgram();
    program->shaders = newArray(sizeof(GLuint));

    return (program);
}

/*--------------------------------------
 * Function: compileFragmentShader()
 * Parameters:
 *   program  Det shader-program som shadern ska läggas till i.
 *   source   GLSL-koden som ska kompileras och länkas till det specificerade
 *            shader-programmet.
 *
 * Description:
 *   Kompilerar och länkar GLSL-kod till en fragment-shader i det specificerade
 *   shader-programmet.
 *------------------------------------*/
void compileFragmentShader(shaderProgramADT program, string source) {
    compileShader(GL_FRAGMENT_SHADER, program, source);
}

/*--------------------------------------
 * Function: compileVertexShader()
 * Parameters:
 *   program  Det shader-program som shadern ska läggas till i.
 *   source   GLSL-koden som ska kompileras och länkas till det specificerade
 *            shader-programmet.
 *
 * Description:
 *   Kompilerar och länkar GLSL-kod till en vertex-shader i det specificerade
 *   shader-programmet.
 *------------------------------------*/
void compileVertexShader(shaderProgramADT program, string source) {
    compileShader(GL_VERTEX_SHADER, program, source);
}


static shaderProgramADT curr_shader_program;

/*--------------------------------------
 * Function: setShaderUniform()
 * Parameters:
 *   name   Namnet på uniform-variabeln.
 *   type   Uniform-variabelns datatype.
 *   value  Värdet som uniform-variabeln ska tilldelas.
 *
 * Description:
 *   Sätter den specificerade uniform-variabeln till det specificerade värdet.
 *   Se nyckelordet uniform i språkspecifikationen för GLSL för mer information.
 *------------------------------------*/
void setShaderUniform(string name, uniformTypeT type, const void *value) {
    GLint loc = glGetUniformLocation(curr_shader_program->id, name);

    switch (type) {
    case FloatUniform:
        glUniform1f(loc, *(float *)value);
        break;
    case Matrix4Uniform:
        glUniformMatrix4fv(loc, 1, GL_TRUE, value);
        break;
    default:
        error("Unknown uniform type specified");
    }
}


/*--------------------------------------
 * Function: useShaderProgram()
 * Parameters:
 *   program  Det shader-program som ska användas för nästkommande ritoperationer.
 *
 * Description:
 *   Aktiverar det specificerade shader-programmet.
 *------------------------------------*/
void useShaderProgram(shaderProgramADT program) {
    glUseProgram(program ? program->id : 0);
    curr_shader_program = program;
}

/*--------------------------------------
 * Function: deleteShaderProgram()
 * Parameters:
 *   program  Det shader-program som ska tas bort.
 *
 * Description:
 *   Tar bort det specificerade shader-programmet inklusive alla tillhörande
 *   shaders.
 *------------------------------------*/
void deleteShaderProgram(shaderProgramADT program) {
    for (int i = 0; i < program->shaders->num_elems; i++) {
        GLuint shader_id;
        arrayGet(program->shaders, i, &shader_id);
        glDeleteShader(shader_id);
    }

    useShaderProgram(NULL);

    glDeleteProgram(program->id);

    freeArray(program->shaders);
    free(program);
}

/*--------------------------------------
 * Function: setFrameRate()
 * Parameters:
 *   fps  The number of frames to display each second.
 *
 * Description:
 *   Sets the number of frames to display each second. Specify a framerate of
 *   zero to disable fps synchronization.
 *
 * Usage:
 *   setFrameRate(60.0f);
 *------------------------------------*/
void setFrameRate(float fps) {
    checkGraphicsInited();

    // Zero fps means we disable fps synchronization.
    if (fps == 0.0f) {
        window->frame_time = UnlimitedFPS;
        return;
    }

    LARGE_INTEGER freq;
    assert(QueryPerformanceFrequency(&freq));

    window->frame_time = (int)(freq.QuadPart / fps);

    assert(QueryPerformanceCounter(&window->last_update));
}

/*------------------------------------------------------------------------------
 * Funktioner för att rensa ritytan, rita geomtriska figurer och texturer samt
 * för att presentera ritytan på skärmen m.m.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: createBox()
 * Parameters:
 *   width   The width of the box (along the x-axis).
 *   height  The height of the box (y-axis).
 *   length  The length of the box (z-axis).
 *
 * Returns:
 *   A pointer to the generated mesh. The pointer actually points to a triMeshT_
 *   structure which contains private fields, but we hide them to the client by
 *   casting to the public type triMeshT instead. 
 *
 * Description:
 *   Creates a box mesh by laying out the vertices and attaching the triangles.
 *   The six sides of the box do not share vertices, because if they are shared,
 *   OpenGL interpolates the normals, effectively resulting in a six sided
 *   sphere, which is not what we want.
 *
 * Usage:
 *   triMeshT *cube = createBox(1.0f, 1.0f, 1.0f);
 *------------------------------------*/
triMeshT *createBox(float width, float height, float length) {
    // Divide the measurements in half since we span the box in both ways per
    // dimension.
    width  /= 2.0f;
    height /= 2.0f;
    length /= 2.0f;

    triMeshT_ *box = malloc(sizeof(triMeshT_));

    // A box has eight corners, but every side needs their own normals for flat-
    // shading to work properly (we don't want interpolated normals). Since each
    // vertex has its own normal, we need six sides times four vertices.
    box->num_verts = 6 * 4;
    box->verts     = malloc(sizeof(vertexT) * box->num_verts);

    // A box has six sides, and every side is made up of two triangles.
    box->num_tris = 6 * 2;
    box->tris     = malloc(sizeof(triT) * box->num_tris);

    // Alias pointers for less code clutter.
    vertexT *v = box->verts;
    triT    *t = box->tris;

    // Front.
    v[0].p = (vec3) {  width,  height, length };
    v[1].p = (vec3) { -width,  height, length };
    v[2].p = (vec3) { -width, -height, length };
    v[3].p = (vec3) {  width, -height, length };

    v[0].n = v[1].n = v[2].n = v[3].n = (vec3) { 0.0f, 0.0f, 1.0f };

    t[0] = (triT) { 0, 1, 2 };
    t[1] = (triT) { 2, 3, 0 };

    // Right.
    v[4].p = (vec3) { width,  height, -length };
    v[5].p = (vec3) { width,  height,  length };
    v[6].p = (vec3) { width, -height,  length };
    v[7].p = (vec3) { width, -height, -length };

    v[4].n = v[5].n = v[6].n = v[7].n = (vec3) { 1.0f, 0.0f, 0.0f };

    t[2] = (triT) { 4, 5, 6 };
    t[3] = (triT) { 6, 7, 4 };

    // Back.
    v[ 8].p = (vec3) { -width,  height, -length };
    v[ 9].p = (vec3) {  width,  height, -length };
    v[10].p = (vec3) {  width, -height, -length };
    v[11].p = (vec3) { -width, -height, -length };

    v[8].n = v[9].n = v[10].n = v[11].n = (vec3) { 0.0f, 0.0f, -1.0f };

    t[4] = (triT) { 8,  9, 10 };
    t[5] = (triT) { 10, 11, 8 };

    // Left.
    v[12].p = (vec3) { -width,  height,  length };
    v[13].p = (vec3) { -width,  height, -length };
    v[14].p = (vec3) { -width, -height, -length };
    v[15].p = (vec3) { -width, -height,  length };

    v[12].n = v[13].n = v[14].n = v[15].n = (vec3) { -1.0f, 0.0f, 0.0f };

    t[6] = (triT) { 12, 13, 14 };
    t[7] = (triT) { 14, 15, 12 };

    // Top.
    v[16].p = (vec3) {  width, height, -length };
    v[17].p = (vec3) { -width, height, -length };
    v[18].p = (vec3) { -width, height,  length };
    v[19].p = (vec3) {  width, height,  length };

    v[16].n = v[17].n = v[18].n = v[19].n = (vec3) { 0.0f, 1.0f, 0.0f };

    t[8] = (triT) { 16, 17, 18 };
    t[9] = (triT) { 18, 19, 16 };

    // Bottom
    v[20].p = (vec3) {  width, -height,  length };
    v[21].p = (vec3) { -width, -height,  length };
    v[22].p = (vec3) { -width, -height, -length };
    v[23].p = (vec3) {  width, -height, -length };

    v[20].n = v[21].n = v[22].n = v[23].n = (vec3) { 0.0f, -1.0f, 0.0f };

    t[10] = (triT) { 20, 21, 22 };
    t[11] = (triT) { 22, 23, 20 };

    // Time to upload the box to the GPU via OpenGL.

    size_t vb_size = sizeof(vertexT) * box->num_verts;
    size_t ib_size = sizeof(triT)    * box->num_tris;

    glGenBuffers(1, &box->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, box->vbo);
    glBufferData(GL_ARRAY_BUFFER, vb_size, box->verts, GL_STATIC_DRAW);


    glGenBuffers(1, &box->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, box->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ib_size, box->tris, GL_STATIC_DRAW);

    return ((triMeshT *)box);
}

/*--------------------------------------
 * Function: updateMesh()
 * Parameters:
 *   mesh  The mesh to update.
 *
 * Description:
 *   Updates the specified mesh in VRAM by reuploading it.
 *
 * Usage:
 *   updateMesh(my_mesh);
 *------------------------------------*/
void updateMesh(const triMeshT *mesh) {
    size_t vb_size = sizeof(vertexT) * mesh->num_verts;
    size_t ib_size = sizeof(triT)    * mesh->num_tris;

    glBindBuffer(GL_ARRAY_BUFFER, ((triMeshT_ *)mesh)->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vb_size, mesh->verts);

    glBindBuffer(GL_ARRAY_BUFFER, ((triMeshT_ *)mesh)->ibo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, ib_size, mesh->tris);
}

/*--------------------------------------
 * Function: deleteMesh()
 * Parameters:
 *   mesh  The mesh to delete.
 *
 * Description:
 *   Deletes the specified mesh from RAM and VRAM.
 *
 * Usage:
 *   deleteMesh(my_mesh);
 *------------------------------------*/
void deleteMesh(triMeshT *mesh) {
    glDeleteBuffers(1, ((triMeshT_ *)mesh)->vbo);
    glDeleteBuffers(1, ((triMeshT_ *)mesh)->ibo);

    free(mesh->verts);
    free(mesh->tris);
    free(mesh);
}

/*--------------------------------------
 * Function: clearDisplay()
 * Parameters:
 *   r  Red color component.
 *   g  Green color component.
 *   b  Blue color component..
 *
 * Description:
 *   Clears the display to the specified color.
 *
 * Usage:
 *   clearDisplay(1.0f, 1.0f, 1.0f);
 *------------------------------------*/
void clearDisplay(float r, float g, float b) {
    checkGraphicsInited();

    glClearColor(r, g, b, 1.0f);
    glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*--------------------------------------
 * Function: drawMesh()
 * Parameters:
 *   mesh  The mesh to draw.
 *
 * Description:
 *   Draws the specified mesh. Remember to assign a shader program with the
 *   useShaderProgram() function.
 *
 * Usage:
 *   drawMesh(my_mesh);
 *------------------------------------*/
void drawMesh(const triMeshT *mesh) {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, ((triMeshT_ *)mesh)->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexT), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertexT), &((vertexT *)NULL)->n);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((triMeshT_ *)mesh)->ibo);
    glDrawElements(GL_TRIANGLES, mesh->num_tris*3, GL_UNSIGNED_INT, (void *)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

/*--------------------------------------
 * Function: updateDisplay()
 * Parameters:
 *
 * Description:
 *   Updates the display by processing pending window events and swapping the
 *   graphics buffer into the graphics window. The function also waits a certain
 *   amount of time before returning, to maintain the FPS specified with the
 *   setFrameRate() function.
 *
 * Usage:
 *   updateDisplay();
 *------------------------------------*/
void updateDisplay(void) {
    checkGraphicsInited();

    // Swap the buffer into the window if there's a device context handle.
    if (window->hdc)
        assert(SwapBuffers(window->hdc));

    // We enter a loop and stay in it just long enough to time and synchronize
    // the FPS, so we get a set amount of frames displayed each second.
    LARGE_INTEGER perf_count;
    do {
        // The window will hang if we don't process pending messages. The
        // DispatchMessageW() function sends the messages to the WindowProc()
        // function, where they will be processed.
        MSG msg;
        while (window && PeekMessageW(&msg, window->hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        // If window is NULL here, the user has closed the window and the
        // exitGraphics() function has been called by the WindowProc() function,
        // which, in turn, has been invoekd by the DispatchMessageW() function.
        if (!window)
            return;

        // We calculate how much time has passed since our latest display
        // update. If enough time has passed, we've displayed this frame exactly
        // the amount of time needed to maintain a set frame interval, so we can
        // exit the loop.
        assert(QueryPerformanceCounter(&perf_count));
        perf_count.QuadPart -= window->last_update.QuadPart;
    } while (perf_count.QuadPart < window->frame_time);

    assert(QueryPerformanceCounter(&window->last_update));
}

/*------------------------------------------------------------------------------
 * Funktioner för att läsa ut inställningar och information om grafikläget m.m.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: windowIsOpen()
 * Parameters:
 *
 * Returns:
 *   True if the graphics window is still open.
 *
 * Description:
 *   Returns true if the graphics window is open.
 *
 * Usage:
 *   bool is_open = windowIsOpen();
 *------------------------------------*/
bool windowIsOpen(void) {
    return (window != NULL);
}
