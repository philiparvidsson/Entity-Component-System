#ifdef __linux__

/*------------------------------------------------------------------------------
 * File: graphics.c
 * Created: June 8, 2015
 * Last changed: June 20, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   This file implements a graphics library using modern OpenGL, supporting
 *   shaders and much more, using a right-handed coordinate system.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"
#include "base/debug.h"
#include "graphics/graphics.h"

#include <X11/Xlib.h>

//#include <GL/glew.h>
//#include <GL/glxew.h>
#include <GL/glew.h>
#include <GL/glxew.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: DefaultFPS
 *
 * Description:
 *   The number of frames to display each second, by default. This can be
 *   configured with the setFrameRate() function.
 *------------------------------------*/
#define DefaultFPS (60.0f)

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: windowT
 *
 * Description:
 *   Represents the graphics window.
 *------------------------------------*/
typedef struct windowT {
    const string* title;      // Window title.
    int           width,      // Window width, in pixels.
                  height;     // Window height, in pixels.
    int           frame_time;

    // Below are platform specific system fields.

    Atom wm_delete_msg;
    Display* display;
    Window* root;
    XVisualInfo* xvi;
    Colormap colormap;
    Window* window;
    GLXContext glx;
} windowT;

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

static windowT* window = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/
void initGLX(void)
{
    glXChooseFBConfig =
        (PFNGLXCHOOSEFBCONFIGPROC)
        glXGetProcAddress((GLubyte*)"glXChooseFBConfig");
    glXGetVisualFromFBConfig =
        (PFNGLXGETVISUALFROMFBCONFIGPROC)
        glXGetProcAddress((GLubyte*)"glXGetVisualFromFBConfig");
    glXCreateContextAttribsARB =
        (PFNGLXCREATECONTEXTATTRIBSARBPROC)
        glXGetProcAddress((GLubyte*)"glXCreateContextAttribsARB");
}

static void createWindow(const string* title, int width, int height) {
    initGLX();

    window = malloc(sizeof(windowT));

    window->display = XOpenDisplay(0);
    if (!window->display)
        error("could not connect to x server");

    window->root = DefaultRootWindow(window->display);
    if (!window->root)
        error("could not get root window");

    int config_attr[] = {
        GLX_RENDER_TYPE  , GLX_RGBA_BIT,
        GLX_X_RENDERABLE , True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER , True,
        GLX_RED_SIZE     , 8,
        GLX_BLUE_SIZE    , 8,
        GLX_GREEN_SIZE   , 8,
        None
    };

    int num_configs;
    GLXFBConfig* configs = glXChooseFBConfig(window->display, DefaultScreen(window->display), config_attr, &num_configs);
    window->xvi = glXGetVisualFromFBConfig(window->display, configs[0]);

    window->colormap = XCreateColormap(window->display, window->root, window->xvi->visual, AllocNone);

    XSetWindowAttributes xswa;
    xswa.colormap   = window->colormap;
    xswa.event_mask = KeyPressMask;

    window->window = XCreateWindow(window->display, window->root, 0, 0, width, height, 0, window->xvi->depth, InputOutput, window->xvi->visual, CWColormap | CWEventMask, &xswa);

    XMapWindow(window->display, window->window);
    XStoreName(window->display, window->window, title);

    int context_attr[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        GLX_CONTEXT_PROFILE_MASK_ARB , GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    window->glx = glXCreateContextAttribsARB(window->display, configs[0], 0, True, context_attr);
    glXMakeCurrent(window->display, window->window, window->glx);

    window->wm_delete_msg = XInternAtom(window->display, "WM_DELETE_WINDOW", False);
}

static void destroyWindow(void) {

}

void updateWindow(void) {
    XEvent xe;
    //XNextEvent(display, &xe);
    if (XCheckWindowEvent(window->display, window->window, KeyPressMask, &xe)) {
        if (xe.type == ClientMessage) {
            if (xe.xclient.data.l[0] == window->wm_delete_msg) {
                // Easy h4x, lol.
                window = NULL;
            }
        }
    }
}

/*--------------------------------------
 * Function: setFrameRate(fps)
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
static void setFrameRate(float fps) {
    // Zero fps means we disable fps synchronization.
    if (fps == 0.0f) {
        window->frame_time = -1;
        return;
    }
}

void initGraphics(const string* title, int width, int height) {
    createWindow(title, width, height);

    glewExperimental = GL_TRUE;
    assert(glewInit() == GLEW_OK);
    assert(GLEW_VERSION_3_3);

    trace("");
    trace("OpenGL vendor: %s" , glGetString(GL_VENDOR));
    trace("OpenGL version: %s", glGetString(GL_VERSION));
    trace("GLSL version: %s"  , glGetString(GL_SHADING_LANGUAGE_VERSION));
    trace("");

    // We need to enable GL_BLEND for transparency.
    glEnable   (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Ignore triangles that are "looking away" from the camera.
    glEnable(GL_CULL_FACE);

    // Enable z-buffering and make sure it's right-handed (i.e. -z is into the
    // screen).
    glEnable    (GL_DEPTH_TEST);
    glClearDepth(0.0f);
    glDepthFunc (GL_GREATER);

    setFrameRate(DefaultFPS);
}

/*--------------------------------------
 * Function: exitGraphics()
 * Parameters:
 *
 * Description:
 *   Closes the window and exits graphics mode.
 *
 * Usage:
 *   exitGraphics();
 *------------------------------------*/
void exitGraphics(void) {
    destroyWindow();
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
    glClearColor(r, g, b, 1.0f);
    glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    if (!window)
        return;

    glXSwapBuffers(window->display, window->window);

    updateWindow();
}

void hideWindow(void) {
}

void showWindow(void) {
    assert(window != NULL);
}

int screenWidth() {
    return window->width;
}

int screenHeight() {
    return window->height;
}

bool windowIsFocused(void) {
    return (window);
}

bool windowIsOpen(void) {
    return (window);
}

#endif // __linux__
