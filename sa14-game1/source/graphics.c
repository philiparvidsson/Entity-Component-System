/*------------------------------------------------------------------------------
 * File: graphics.c
 * Created: June 8, 2015
 * Last changed: June 13, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder ett enkelt grafikbibliotek f�r att rita och visa grafik. Biblio-
 *   teket anv�nder OpenGL och renderar s�ledes med h�rdvara om m�jlighet finns.
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
 *   Namnet p� klassen som anv�nds f�r att skapa grafikf�nstret.
 *------------------------------------*/
#define ClassName (L"sa14-game1")

/*--------------------------------------
 * Constant: DefaultFPS
 *
 * Description:
 *   Antal bildrutor som ska visas per sekund. Detta g�r att �ndra med
 *   setFrameRate()-funktionen.
 *------------------------------------*/
#define DefaultFPS (30.0f)

/*--------------------------------------
 * Constant: UnlimitedFPS
 *
 * Description:
 *   Obegr�nsat antal bildrutor per sekund.
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
 *   Abstrakt pekartyp f�r ett shader-program.
 *------------------------------------*/
struct shaderProgramCDT {
    GLuint  id;      /* Shader-programmets interna ID-nummer. */
    arrayT *shaders; /* Pekare till array med programmets shaders. */
};

/*--------------------------------------
 * Type: windowT
 *
 * Description:
 *   Datatyp som representerar ett f�nster.
 *------------------------------------*/
typedef struct {
    const char* title;      /* F�nstrets titel.                      */
    int         width,      /* F�nstrets bredd i antal pixlar.       */
                height;     /* F�nstrets h�jd i antal pixlar.        */
    int         frame_time; /* Den tid som varje bildruta ska visas. */

    /* Nedan �r plattformsspecifika, systemrelaterade variabler. */

    HWND          hwnd;        /* Systemets egna "handtag" till f�nstret.    */
    HDC           hdc;         /* Den DC (device context) som anv�nds.       */
    HGLRC         hglrc;       /* Den renderingskontext som anv�nds.         */
    LARGE_INTEGER last_update; /* Tidsst�mpel f�r senaste bilduppdateringen. */
} windowT;

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

/*--------------------------------------
 * Variable: window
 *
 * Description:
 *   Pekare till grafikf�nstret.
 *------------------------------------*/
static windowT *window = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*------------------------------------------------------------------------------
 * Nedan �r de privata funktionerna som inte �r t�nkta att anv�ndas utanf�r
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
        /* Anv�ndaren har st�ngt f�nstret, s� vi avslutar grafikl�get. */
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
 *   Avslutar programmet med ett felmeddelande om grafikl�get inte �r initierat.
 *------------------------------------*/
static void checkGraphicsInited(void) {
    if (!window)
        error("Graphics not initialized");
}

/*--------------------------------------
 * Function: compileShader()
 * Parameters:
 *   type    Den typ av shader som ska kompileras.
 *   prog    Shader-programmet som shadern ska kompileras in i och l�nkas till.
 *   source  Shader-programmets GLSL-kod..
 *
 * Description:
 *   Kompilerar en shader och l�nkar den till det specificerade shader-
 *   programmet.
 *------------------------------------*/
static void compileShader(GLenum type, shaderProgramADT prog, string source) {
    GLuint shader_id = glCreateShader(type);

    glShaderSource (shader_id, 1, &source, NULL);
    glCompileShader(shader_id);

    /*
     * Om GL_COMPILE_STATUS returnerar GL_FALSE i result-parametern s� miss-
     * lyckades kompileringen, f�rmodligen p� grund av trasig kod. Vi kan inte
     * forts�tta d� utan genererar ett fel ist�llet.
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
 *   title   F�nstrets titel.
 *   width   F�nstrets bredd, i antal pixlar.
 *   height  F�nstrets h�jd, i antal pixlar.
 *
 * Description:
 *   Skapar ett grafikf�nster med de specificerade dimensionerna. Den angivna
 *   bredden och h�jden inkluderar inte f�nsterdekorationer, utan endast
 *   storleken p� klientytan.
 *------------------------------------*/
static void createWindow(string title, int width, int height) {
    RECT  rect  = { 0 }; rect.right = width; rect.bottom = height;
    DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

    /*
     * H�r justerar vi storleken p� f�nstret s� att de angivna dimensionerna
     * inte inkluderar f�nsterdekorationer s� som kantomr�den, titelomr�de m.m.
     */
    assert(AdjustWindowRectEx(&rect, style, FALSE, WS_EX_LEFT));

    /* Dags att allokera f�nsterdatatypen. */
    window = malloc(sizeof(windowT));

    /*
     * Eftersom CreateWindowExW()-funktionen vill ha Unicode-str�ngar, s� m�ste
     * vi konvertera titel-str�ngen till en Unicode-str�ng och skicka den till
     * funktionen.
     */
    size_t   window_name_length = mbstowcs(NULL, title, 0) + 1;
    wchar_t *window_name        = malloc(sizeof(wchar_t) * window_name_length);

    mbstowcs(window_name, title, window_name_length);

    /* H�r skapar vi f�nstret med ett anrop ner i Windows API. */
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
     * CreateWindowEx()-funktionen kopierar titel-str�ngen s� det �r ok att
     * deallokera den h�r.
     */
    free(window_name);

    /* Om window->hwnd �r NULL s� skapades inget f�nster. */
    assert(window->hwnd != NULL);

    /* Utan detta anrop syns inte f�nstret. */
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
 *   Registrerar f�nsterklassen.
 *------------------------------------*/
static void registerWindowClass(void) {
    /*
     * Innan vi kan skapa f�nster kr�ver Windows att vi registrerar en f�nster-
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
 *   Initierar grafikl�ge f�r f�nstret.
 *------------------------------------*/
static void setupPixelFormat(void) {
    /* Detta kr�vs f�r att ett f�nster ska acceptera OpenGL-l�ge. */

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
 *   Avregistrerar f�nsterklassen.
 *------------------------------------*/
static void unregisterWindowClass(void) {
    assert(UnregisterClassW(ClassName, GetModuleHandleW(NULL)));
}

/*------------------------------------------------------------------------------
 * H�r nedanf�r �r de publika funktionerna som �r t�nkta att anv�ndas av klient-
 * programmen f�r att initiera grafikl�get m.m.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Funktioner f�r att initiera grafikl�ge, avsluta grafikl�ge samt konfigurera
 * grafikl�get och st�lla in parametrar.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: initGraphics()
 * Parameters:
 *   title   F�nstrets titel.
 *   width   F�nstrets bredd, i antal pixlar.
 *   height  F�nstrets h�jd, i antal pixlar.
 *
 * Description:
 *   Skapar ett grafikf�nster med de specificerade dimensionerna. Den angivna
 *   bredden och h�jden inkluderar inte f�nsterdekorationer, utan endast
 *   storleken p� klientytan.
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
     * Vi s�tter p� GL_BLEND s� att alpha-kanalen anv�nds, d.v.s. transparens
     * blir m�jligt.
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
 *   St�nger grafikf�nstret.
 *------------------------------------*/
void exitGraphics(void) {
    /* Om window redan �r NULL s� �r vi inte i grafikl�ge. */
    if (!window)
        return;

    assert(wglMakeCurrent(window->hdc, NULL));
    assert(wglDeleteContext(window->hglrc));

    window->hdc   = NULL;
    window->hglrc = NULL;

    assert(DestroyWindow(window->hwnd));

    /* @To-do: �r det en bra id� att anropa updateWindow() h�r? */
    updateDisplay();

    /* Vi st�dar upp efter oss genom att avregistrera f�nsterklassen h�r. */
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
 *   program  Det shader-program som shadern ska l�ggas till i.
 *   source   GLSL-koden som ska kompileras och l�nkas till det specificerade
 *            shader-programmet.
 *
 * Description:
 *   Kompilerar och l�nkar GLSL-kod till en fragment-shader i det specificerade
 *   shader-programmet.
 *------------------------------------*/
void compileFragmentShader(shaderProgramADT program, string source) {
    compileShader(GL_FRAGMENT_SHADER, program, source);
}

/*--------------------------------------
 * Function: compileVertexShader()
 * Parameters:
 *   program  Det shader-program som shadern ska l�ggas till i.
 *   source   GLSL-koden som ska kompileras och l�nkas till det specificerade
 *            shader-programmet.
 *
 * Description:
 *   Kompilerar och l�nkar GLSL-kod till en vertex-shader i det specificerade
 *   shader-programmet.
 *------------------------------------*/
void compileVertexShader(shaderProgramADT program, string source) {
    compileShader(GL_VERTEX_SHADER, program, source);
}

/*--------------------------------------
 * Function: setShaderUniform()
 * Parameters:
 *   program  Det shader-program vars parametrar ska st�llas in.
 *   name     Namnet p� uniform-variabeln.
 *   val      V�rdet som uniform-variabeln ska tilldelas.
 *
 * Description:
 *   S�tter den specificerade uniform-variabeln till det specificerade v�rdet.
 *   Se nyckelordet uniform i spr�kspecifikationen f�r GLSL f�r mer information.
 *------------------------------------*/
void setShaderUniform(shaderProgramADT program, string name, float value) {
    glUniform1f(glGetUniformLocation(program->id, name), value);
}

/*--------------------------------------
 * Function: useShaderProgram()
 * Parameters:
 *   program  Det shader-program som ska anv�ndas f�r n�stkommande ritoperationer.
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
 *   Tar bort det specificerade shader-programmet inklusive alla tillh�rande
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
*   St�ller in hur m�nga bildrutor som ska visas per sekund. Ange noll f�r
*   obegr�nsat antal.
*------------------------------------*/
void setFrameRate(float fps) {
    checkGraphicsInited();

    /*
     * Om man anger noll fps s� st�nger vi av synkroniseringen genom att s�tta
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
 * Funktioner f�r att rensa ritytan, rita geomtriska figurer och texturer samt
 * f�r att presentera ritytan p� sk�rmen m.m.
 *----------------------------------------------------------------------------*/

geometryT *createBox(float width, float height, float length) {
    /*
     * Vi delar dimensionerna i h�lften eftersom vi kommer att anv�nda dem f�r
     * att konstruera l�dan nedan och d� anv�nder -width till width som bredden
     * p� den. D.v.s. den skulle bli dubbelt s� stor som �nskat annars.
     */
    width  /= 2.0f;
    height /= 2.0f;
    length /= 2.0f;

    geometryT_ *box = malloc(sizeof(geometryT_));

    /* En l�da har sex sidor, och varje sida best�r av tv� trianglar. */
    box->tris     = malloc(sizeof(triT) * 12);
    box->num_tris = 12;

    /*
     * En l�da har �tta h�rn, men varje sida vill ha egna h�rn. Vi f�r allts�
     * fyra h�rn per sida, och sex sidor, d.v.s. 24 h�rn.
     */
    box->normals   = malloc(sizeof(vector3T) * 24);
    box->verts     = malloc(sizeof(vector3T) * 24);
    box->num_verts = 8;

    /* Alias-pekare bara f�r att f� lite enklare kod nedan. */
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

    /* H�gersida. */
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

    /* V�nstersida. */
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
 *   r  R�d f�rgkomponent.
 *   g  Gr�n f�rgkomponent.
 *   b  Bl� f�rgkomponent.
 *
 * Description:
 *   Rensar ritytan til den specificerade f�rgen.
 *------------------------------------*/
void clearDisplay(float r, float g, float b) {
    checkGraphicsInited();

    glClearColor(r, g, b, 1.0f);
    glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*--------------------------------------
 * Function: setColor()
 * Parameters:
 *   r  R�d f�rgkomponent.
 *   g  Gr�n f�rgkomponent.
 *   b  Bl� f�rgkomponent.
 *   a  Alpha-v�rde.
 *
 * Description:
 *   �ndrar f�rg f�r n�stkommande anrop till ritfunktioner.
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
 *   Uppdaterar grafikf�nstret.
 *------------------------------------*/
void updateDisplay(void) {
    checkGraphicsInited();

    /* H�r presenterar vi ritytan i f�nstret genom att "swappa" in bufferten. */
    if (window->hdc)
        assert(SwapBuffers(window->hdc));

    /*
     * Nedan snurrar vi i en loop tills vi pausat s� l�nge som beh�vs f�r att
     * uppr�tth�lla r�tt intervall f�r antal bildrutor per sekund.
     */
    LARGE_INTEGER perf_count;
    do {
        /*
         * H�r ser vi till att f�nstret inte h�nger sig genom att ta emot och
         * hantera f�nstermeddelanden. Funktionen DispatchMessageW() skickar dem
         * vidare, varp� de blir hanterade i WindowProc()-funktionen.
         */
        MSG msg;
        while (window && PeekMessageW(&msg, window->hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        /*
         * Om window �r NULL h�r s� har anv�ndaren st�ngt f�nstret, varp�
         * exitGraphics()-funktionen anropats av WindowProc()-funktionen, som i
         * sin tur anropats av DispatchMessageW()-funktionen.
         */
        if (!window)
            return;

        /*
         * Vi r�knar ut den tid som g�tt sedan den senate bilduppdateringen, och
         * om tillr�ckligt mycket tid passerat s� g�r vi ur loopen d� vi visat
         * denna bildruta precis s� l�nge som beh�vs f�r att synkronisera
         * antalet bildrutor per sekund till r�tt antal.
         */
        assert(QueryPerformanceCounter(&perf_count));
        perf_count.QuadPart -= window->last_update.QuadPart;
    } while (perf_count.QuadPart < window->frame_time);

    assert(QueryPerformanceCounter(&window->last_update));
}

/*------------------------------------------------------------------------------
 * Funktioner f�r att l�sa ut inst�llningar och information om grafikl�get m.m.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: windowIsOpen()
 * Parameters:
 *
 * Returns:
 *   Sant om grafikf�nstret �r �ppet, annars falskt.
 *
 * Description:
 *   Returnerar sant om grafikf�nstret �r �ppet.
 *------------------------------------*/
bool windowIsOpen(void) {
    return (window != NULL);
}
