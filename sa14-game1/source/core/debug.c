/*------------------------------------------------------------------------------
 * File: debug.c
 * Created: June 8, 2015
 * Last changed: June 13, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Makron och funktioner för debugging.
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "debug.h"

#include "core/common.h"

#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: exitWithMessage()
 * Parameters:
 *   msg        Meddelandet som ska visas.
 *   func_name  Namnet på den funktion som programmet kraschade i.
 *   line       Den rad i koden som programmet kraschade på.
 *
 * Description:
 *   Avslutar programmet med ett meddelande.
 *------------------------------------*/
void exitWithMessage(string msg, string func_name, int line) {
    printf("\n----------------------------------------\n"
           "ERROR: %s in %s() on line %d.\n\n"
           "This program will now exit.\n", msg, func_name, line);

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
    }

    printf("Press ENTER to continue...");
    getchar();
#ifdef _MSC_VER
    __debugbreak();
#endif
    exit(EXIT_FAILURE);
}
