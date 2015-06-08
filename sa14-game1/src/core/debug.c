/*------------------------------------------------------------------------------
 * File: debug.c
 * Created: June 8, 2015
 * Last changed: June 8, 2015
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

#include <stdio.h>
#include <stdlib.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: exitWithMessage()
 * Parameters:
 *   message    Meddelandet som ska visas.
 *   func_name  Namnet på den funktion som programmet kraschade i.
 *   line       Den rad i koden som programmet kraschade på.
 *
 * Description:
 *   Avslutar programmet med ett meddelande.
 *------------------------------------*/
void exitWithMessage(stringT message, stringT func_name, int line) {
    printf("\n----------------------------------------\n"
           "ERROR: %s in %s() on line %d.\n\n"
           "This program will now crash.\n"
           "Press ENTER to continue...", message, func_name, line);
    getchar();
#ifdef _MSC_VER
    __debugbreak();
#endif
    exit(EXIT_FAILURE);
}
