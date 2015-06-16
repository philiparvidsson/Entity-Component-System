/*------------------------------------------------------------------------------
 * File: debug.c
 * Created: June 8, 2015
 * Last changed: June 16, 2015
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

#include "core/common.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#endif

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: exitWithErrorMsg()
 * Parameters:
 *   msg        The error message to display.
 *   func_name  The name of the function that generated the error.
 *   line       The number of the line that generated the error.
 *
 * Description:
 *   Displays the specified error message and exits the program. This function
 *   is intended to be called by the debugging macros and you should generally
 *   not call it directly.
 *
 * Usage:
 *   exitWithErrorMsg("An error has occurred");
 *
 *------------------------------------*/
void exitWithErrorMsg(string msg, string func_name, int line) {
    printf("\n----------------------------------------\n"
           "ERROR: %s in %s() on line %d.\n\n"
           "This program will now exit.\n", msg, func_name, line);

#ifdef _WIN32
    int last_error = GetLastError();
    if (last_error != NO_ERROR) {
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
#endif // _WIN32
    }

    printf("Press ENTER to continue...");
    getchar();
#ifdef _MSC_VER
    __debugbreak();
#endif
    exit(EXIT_FAILURE);
}
