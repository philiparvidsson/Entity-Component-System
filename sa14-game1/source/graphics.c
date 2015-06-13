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

#include <stdlib.h>

#include <glew.h>

#include <Windows.h>
#include <GL/gl.h>

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

struct shaderProgramCDT {
    GLuint id;
    arrayT *shaders;
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

static void loadShader(GLenum type, shaderProgramADT p, string source) {
    GLuint shader_id = glCreateShader(type);

    glShaderSource(shader_id, 1, &source, NULL);
    glCompileShader(shader_id);
    glAttachShader(p->id, shader_id);

    arrayAdd(p->shaders, &shader_id);

    glLinkProgram(p->id);
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

shaderProgramADT createShaderProgram() {
    shaderProgramADT p = malloc(sizeof(struct shaderProgramCDT));

    p->id      = glCreateProgram();
    p->shaders = newArray(sizeof(GLuint));

    return (p);
}

void loadFragmentShader(shaderProgramADT p, string source) {
    loadShader(GL_FRAGMENT_SHADER, p, source);
}

void loadVertexShader(shaderProgramADT p, string source) {
    loadShader(GL_VERTEX_SHADER, p, source);
}

void useShaderProgram(shaderProgramADT p) {
    glUseProgram(p ? p->id : 0);
}

void setShaderParam(shaderProgramADT p, string name, float value) {
    glUniform1f(glGetUniformLocation(p->id, name), value);
}

void deleteShaderProgram(shaderProgramADT p) {
    for (int i = 0; i < p->shaders->num_elems; i++) {
        GLuint shader_id;
        arrayGet(p->shaders, i, &shader_id);
        glDeleteShader(shader_id);
    }

    glUseProgram(0);
    glDeleteProgram(p->id);

    freeArray(p->shaders);
    free(p);
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
 *
 * Description:
 *   �ndrar f�rg f�r n�stkommande anrop till ritfunktioner.
 *------------------------------------*/
void setColor(float r, float g, float b) {
    checkGraphicsInited();

    glColor3f(r, g, b);
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
