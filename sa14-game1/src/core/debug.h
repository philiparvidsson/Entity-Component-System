/*------------------------------------------------------------------------------
 * File: debug.h
 * Created: June 8, 2015
 * Last changed: June 8, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Makron och funktioner för debugging.
 *
 *----------------------------------------------------------------------------*/

#ifndef _debug_h_
#define _debug_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

/*------------------------------------------------
 * MACROS
 *----------------------------------------------*/

#define error(msg) exitWithMessage(msg, __FUNCTION__, __LINE__)

#define assert(expr) if (!(expr)) { error("The assertion '" #expr "' failed"); }

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
void exitWithMessage(stringT message, stringT func_name, int line);

#endif // _debug_h_
