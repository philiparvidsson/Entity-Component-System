/*------------------------------------------------------------------------------
 * File: debug.h
 * Created: June 8, 2015
 * Last changed: June 16, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Macros and functions to make debugging easier.
 *----------------------------------------------------------------------------*/

#ifndef debug_h_
#define debug_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

/*------------------------------------------------
 * MACROS
 *----------------------------------------------*/

/*--------------------------------------
 * Macro: error()
 * Parameters:
 *   msg  The error message to display.
 *
 * Description:
 *   Displays the specified message and exits the program.
 *
 * Usage:
 *   error("An error has occurred");
 *------------------------------------*/
#define error(msg) errorExit(msg, __FUNCTION__, __LINE__)

/*--------------------------------------
 * Macro: assert()
 * Parameters:
 *   expr  The expression that will generate an error if it evaluates to false.
 *
 * Description:
 *   Exits the program with a message if the specified expression does not
 *   evaluate to true.
 *
 * Usage:
 *   assert(some_ptr != NULL);
 *------------------------------------*/
#define assert(expr) if (!(expr)) { error("The assertion '" #expr "' failed"); }

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: errorExit()
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
 *   errorExit("An error has occurred", "main.c", 42);
 *
 *------------------------------------*/
void errorExit(string msg, string func_name, int line);

#endif // debug_h_
