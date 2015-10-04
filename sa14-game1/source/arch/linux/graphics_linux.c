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

#include <GL/glew.h>

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
} windowT;

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

static windowT* window = NULL;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void createWindow(const string* title, int width, int height) {

}

static void destroyWindow(void) {

}

/* static */ void updateWindow(void) {

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
