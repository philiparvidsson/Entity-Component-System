/*------------------------------------------------------------------------------
 * File: debug.c
 * Created: June 8, 2015
 * Last changed: July 20, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Macros and functions to make debugging easier.
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "debug.h"

#include "base/common.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#ifdef _WIN32
#include <windows.h>
#endif

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static void printLastErrorGL(void) {
    GLenum error = glGetError();

    if (error == GL_NO_ERROR)
        return;

    printf("\nglGetError() reports: %d", error);

    error = glGetError();
    while (error != GL_NO_ERROR && error != GL_INVALID_OPERATION) {
        printf(", %d", error);
        error = glGetError();
    }

    printf("\n\n");
}

#ifdef _WIN32
static void printLastErrorWin32(void) {
    int last_error = GetLastError();

    if (last_error == NO_ERROR)
        return;

    LPWSTR error_text = NULL;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER
                   | FORMAT_MESSAGE_FROM_SYSTEM
                   | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   last_error,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPWSTR)&error_text,
                   0,
                   NULL);

    wprintf(L"GetLastError() reports %d: %ls\n", last_error, error_text);

    if (error_text)
        LocalFree(error_text);
}
#endif // _WIN32

/*--------------------------------------
 * Function: errorExit(msg, func_name, line, ...)
 * Parameters:
 *   msg        The error message format to display.
 *   func_name  The name of the function that generated the error.
 *   line       The number of the line that generated the error.
 *   ...        The error message format arguments.
 *
 * Description:
 *   Displays the specified error message and exits the program. This function
 *   is intended to be called by the debugging macros and you should generally
 *   not call it directly.
 *
 * Usage:
 *   errorExit("An error has occurred", "main.c", 42);
 *
 *------------------------------------*/
void errorExit(const string* msg, const string* func_name, int line, ...) {
    va_list ap;

    char s[1024];
    va_start(ap, line);
    vsprintf(s, msg, ap);
    va_end(ap);

    printf("\n----------------------------------------\n"
           "ERROR: %s\n\tin %s() on line %d.\n\n"
           "This program will now exit.\n", s, func_name, line);

    printLastErrorGL();

#ifdef _WIN32
    printLastErrorWin32();
#endif // _WIN32

#if defined(_DEBUG) && defined(_MSC_VER)
    printf("Press ENTER to debug...");
    getchar();
    __debugbreak();
#else
    printf("Press ENTER to exit...");
    getchar();
#endif
    exit(EXIT_FAILURE);
}
