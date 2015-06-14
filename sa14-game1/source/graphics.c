/*------------------------------------------------------------------------------
 * File: graphics.c
 * Created: June 8, 2015
 * Last changed: June 13, 2015
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
#include "core/math.h"

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
 *   Namnet på klassen som används för att skapa grafikfönstret.
 *------------------------------------*/
#define ClassName (L"sa14-game1")

/*--------------------------------------
 * Constant: DefaultFPS
 *
 * Description:
 *   Antal bildrutor som ska visas per sekund. Detta går att ändra med
 *   setFrameRate()-funktionen.
 *------------------------------------*/
#define DefaultFPS (30.0f)

/*--------------------------------------
 * Constant: UnlimitedFPS
 *
 * Description:
 *   Obegränsat antal bildrutor per sekund.
 *------------------------------------*/
#define UnlimitedFPS (-1)

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct {
    /* --- Public --- */

    vector3T *verts;
    vector3T *normals;
    int       num_verts;
    triT     *tris;
    int       num_tris;

    /* --- Private --- */

    GLuint vbo, nbo, ibo;
} geometryT_;

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
        /* Användaren har stängt fönstret, så vi avslutar grafikläget. */
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
 *   Avslutar programmet med ett felmeddelande om grafikläget inte är initierat.
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

    /*
     * Om GL_COMPILE_STATUS returnerar GL_FALSE i result-parametern så miss-
     * lyckades kompileringen, förmodligen på grund av trasig kod. Vi kan inte
     * fortsätta då utan genererar ett fel istället.
     */
    GLint result;
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

    /*
     * Vi sätter på GL_BLEND så att alpha-kanalen används, d.v.s. transparens
     * blir möjligt.
     */
    glEnable   (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

/*--------------------------------------
 * Function: setShaderUniform()
 * Parameters:
 *   program  Det shader-program vars parametrar ska ställas in.
 *   name     Namnet på uniform-variabeln.
 *   val      Värdet som uniform-variabeln ska tilldelas.
 *
 * Description:
 *   Sätter den specificerade uniform-variabeln till det specificerade värdet.
 *   Se nyckelordet uniform i språkspecifikationen för GLSL för mer information.
 *------------------------------------*/
void setShaderUniform(shaderProgramADT program, string name, float value) {
    glUniform1f(glGetUniformLocation(program->id, name), value);
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

    glUseProgram(0);
    glDeleteProgram(program->id);

    freeArray(program->shaders);
    free(program);
}

/*--------------------------------------
* Function: setFrameRate()
* Parameters:
*   fps  Antal bildrutor som ska visas per sekund.
*
* Description:
*   Ställer in hur många bildrutor som ska visas per sekund. Ange noll för
*   obegränsat antal.
*------------------------------------*/
void setFrameRate(float fps) {
    checkGraphicsInited();

    /*
     * Om man anger noll fps så stänger vi av synkroniseringen genom att sätta
     * den tid som varje bildruta ska visas till noll.
     */
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

geometryT *createBox(float width, float height, float length) {
    /*
     * Vi delar dimensionerna i hälften eftersom vi kommer att använda dem för
     * att konstruera lådan nedan och då använder -width till width som bredden
     * på den. D.v.s. den skulle bli dubbelt så stor som önskat annars.
     */
    width  /= 2.0f;
    height /= 2.0f;
    length /= 2.0f;

    geometryT_ *box = malloc(sizeof(geometryT_));

    /* En låda har sex sidor, och varje sida består av två trianglar. */
    box->tris     = malloc(sizeof(triT) * 12);
    box->num_tris = 12;

    /*
     * En låda har åtta hörn, men varje sida vill ha egna hörn. Vi får alltså
     * fyra hörn per sida, och sex sidor, d.v.s. 24 hörn.
     */
    box->normals   = malloc(sizeof(vector3T) * 24);
    box->verts     = malloc(sizeof(vector3T) * 24);
    box->num_verts = 8;

    /* Alias-pekare bara för att få lite enklare kod nedan. */
    vector3T *v = box->verts, *n = box->normals;
    triT     *t = box->tris;

    /* Framsida. */
    v[0] = (vector3T) { -width, -height, -length };
    v[1] = (vector3T) { -width,  height, -length };
    v[2] = (vector3T) {  width,  height, -length };
    v[3] = (vector3T) {  width, -height, -length };

    n[0] = n[1] = n[2] = n[3] = (vector3T) { 0.0f, 0.0f, -1.0f };

    t[0] = (triT) { 0, 2, 1 };
    t[1] = (triT) { 0, 3, 2 };

    /* Högersida. */
    v[4] = (vector3T) { width, -height, -length };
    v[5] = (vector3T) { width,  height, -length };
    v[6] = (vector3T) { width,  height,  length };
    v[7] = (vector3T) { width, -height,  length };

    n[4] = n[5] = n[6] = n[7] = (vector3T) { 1.0f, 0.0f, 0.0f };

    t[2] = (triT) { 4, 6, 5 };
    t[3] = (triT) { 4, 7, 6 };

    /* Baksida. */
    v[ 8] = (vector3T) {  width, -height, length };
    v[ 9] = (vector3T) {  width,  height, length };
    v[10] = (vector3T) { -width,  height, length };
    v[11] = (vector3T) { -width, -height, length };

    n[8] = n[9] = n[10] = n[11] = (vector3T) { 0.0f, 0.0f, 1.0f };

    t[4] = (triT) { 8, 10, 9 };
    t[5] = (triT) { 8, 11, 10 };

    /* Vänstersida. */
    v[12] = (vector3T) { -width, -height,  length };
    v[13] = (vector3T) { -width,  height,  length };
    v[14] = (vector3T) { -width,  height, -length };
    v[15] = (vector3T) { -width, -height, -length };

    n[12] = n[13] = n[14] = n[15] = (vector3T) { -1.0f, 0.0f, 0.0f };

    t[6] = (triT) { 12, 14, 13 };
    t[7] = (triT) { 12, 15, 14 };

    /* Ovansida. */
    v[16] = (vector3T) { -width, height, -length };
    v[17] = (vector3T) { -width, height,  length };
    v[18] = (vector3T) {  width, height,  length };
    v[19] = (vector3T) {  width, height, -length };

    n[16] = n[17] = n[18] = n[19] = (vector3T) { 0.0f, 1.0f, 0.0f };

    t[8] = (triT) { 16, 18, 17 };
    t[9] = (triT) { 16, 19, 18 };

    /* Undersida. */
    v[20] = (vector3T) { -width, -height,  length };
    v[21] = (vector3T) { -width, -height, -length };
    v[22] = (vector3T) {  width, -height, -length };
    v[23] = (vector3T) {  width, -height,  length };

    n[20] = n[21] = n[22] = n[23] = (vector3T) { 0.0f, -1.0f, 0.0f };

    t[10] = (triT) { 20, 22, 21 };
    t[11] = (triT) { 20, 23, 22 };


    glGenBuffers(1, &box->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, box->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3T) * 24, box->verts, GL_STATIC_DRAW);

    glGenBuffers(1, &box->nbo);
    glBindBuffer(GL_ARRAY_BUFFER, box->nbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector3T) * 24, box->normals, GL_STATIC_DRAW);


    glGenBuffers(1, &box->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, box->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triT) * 12, box->tris, GL_STATIC_DRAW);

    return ((geometryT *)box);
}

/*--------------------------------------
 * Function: deleteGeometry()
 * Parameters:
 *   geom Den geometri som ska tas bort.
 *
 * Description:
 *   Tar bort den specificerade geometrin.
 *------------------------------------*/
void deleteGeometry(geometryT *geom) {
    free(geom->tris);
    free(geom->verts);
    free(geom);
}

/*--------------------------------------
 * Function: clearDisplay()
 * Parameters:
 *   r  Röd färgkomponent.
 *   g  Grön färgkomponent.
 *   b  Blå färgkomponent.
 *
 * Description:
 *   Rensar ritytan til den specificerade färgen.
 *------------------------------------*/
void clearDisplay(float r, float g, float b) {
    checkGraphicsInited();

    glClearColor(r, g, b, 1.0f);
    glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*--------------------------------------
 * Function: setColor()
 * Parameters:
 *   r  Röd färgkomponent.
 *   g  Grön färgkomponent.
 *   b  Blå färgkomponent.
 *   a  Alpha-värde.
 *
 * Description:
 *   Ändrar färg för nästkommande anrop till ritfunktioner.
 *------------------------------------*/
void setColor(float r, float g, float b, float a) {
    checkGraphicsInited();

    glColor4f(r, g, b, a);
}

void drawGeometry(const geometryT *geom) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, ((geometryT_ *)geom)->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vector3T), (void *)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, ((geometryT_ *)geom)->nbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vector3T), (void *)0);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((geometryT_ *)geom)->ibo);
    glDrawElements(GL_TRIANGLES, geom->num_tris*3, GL_UNSIGNED_INT, (void *)0);

    glDisableVertexAttribArray(0);
}

/*--------------------------------------
 * Function: updateDisplay()
 * Parameters:
 *
 * Description:
 *   Uppdaterar grafikfönstret.
 *------------------------------------*/
void updateDisplay(void) {
    checkGraphicsInited();

    /* Här presenterar vi ritytan i fönstret genom att "swappa" in bufferten. */
    if (window->hdc)
        assert(SwapBuffers(window->hdc));

    /*
     * Nedan snurrar vi i en loop tills vi pausat så länge som behövs för att
     * upprätthålla rätt intervall för antal bildrutor per sekund.
     */
    LARGE_INTEGER perf_count;
    do {
        /*
         * Här ser vi till att fönstret inte hänger sig genom att ta emot och
         * hantera fönstermeddelanden. Funktionen DispatchMessageW() skickar dem
         * vidare, varpå de blir hanterade i WindowProc()-funktionen.
         */
        MSG msg;
        while (window && PeekMessageW(&msg, window->hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        /*
         * Om window är NULL här så har användaren stängt fönstret, varpå
         * exitGraphics()-funktionen anropats av WindowProc()-funktionen, som i
         * sin tur anropats av DispatchMessageW()-funktionen.
         */
        if (!window)
            return;

        /*
         * Vi räknar ut den tid som gått sedan den senate bilduppdateringen, och
         * om tillräckligt mycket tid passerat så går vi ur loopen då vi visat
         * denna bildruta precis så länge som behövs för att synkronisera
         * antalet bildrutor per sekund till rätt antal.
         */
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
 *   Sant om grafikfönstret är öppet, annars falskt.
 *
 * Description:
 *   Returnerar sant om grafikfönstret är öppet.
 *------------------------------------*/
bool windowIsOpen(void) {
    return (window != NULL);
}
