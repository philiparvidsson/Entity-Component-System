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
#include "graphics/graphics.h"

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

static void setTextColor(const string* color) {
#ifdef _WIN32
    HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD   attr    = 0;

    if (!color)
        color = "";

    if (strcmp(color, "red") == 0) {
        attr = FOREGROUND_RED | FOREGROUND_INTENSITY;
    }
    else if (strcmp(color, "white") == 0) {
        attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
             | FOREGROUND_INTENSITY;
    }
    else if (strcmp(color, "yellow") == 0) {
        attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    }
    else {
        attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }

    SetConsoleTextAttribute(std_out, attr);
#endif // _WIN32
}

/*--------------------------------------
 * Function: errorFunc(msg, func_name, line, ...)
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
 *   errorFunc("An error has occurred", "main.c", 42);
 *
 *------------------------------------*/
void errorFunc(const string* msg, const string* func_name, int line, ...) {
    va_list ap;

#ifdef _WIN32
    ShowWindow(GetConsoleWindow(), SW_SHOW);
#endif // _WIN32
    hideWindow();

    string s[1024];
    va_start(ap, line);
    vsprintf(s, msg, ap);
    va_end(ap);

    printf("\n----------------------------------------\n");
    setTextColor("red");
    printf("error: %s\n\tin %s() on line %d.\n\n", s, func_name, line);
    setTextColor(NULL);
    printf("This program will now exit.\n");

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

void traceFunc(const string* msg, ...) {
    va_list ap;

    string s[1024];
    va_start(ap, msg);
    vsprintf(s, msg, ap);
    va_end(ap);

    setTextColor(NULL);
    printf("%s\n", s);
}

void warnFunc(const string* msg, ...) {
    va_list ap;

    string s[1024];
    va_start(ap, msg);
    vsprintf(s, msg, ap);
    va_end(ap);

    setTextColor("yellow");
    printf("warning: %s\n", s);
    setTextColor(NULL);
}
