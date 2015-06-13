/*------------------------------------------------------------------------------
 * File: debug.h
 * Created: June 8, 2015
 * Last changed: June 13, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Makron och funktioner för debugging.
 *
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
 *   msg  Meddelandet som ska visas.
 *
 * Description:
 *   Avslutar programmet med ett meddelande.
 *------------------------------------*/
#define error(msg) exitWithMessage(msg, __FUNCTION__, __LINE__)

/*--------------------------------------
 * Macro: assert()
 * Parameters:
 *   expr  Uttrycket som måste evaluera till sant.
 *
 * Description:
 *   Avslutar programmet med ett meddelande om det givna uttrycket inte
 *   evaluerar till sant.
 *------------------------------------*/
#define assert(expr) if (!(expr)) { error("The assertion '" #expr "' failed"); }

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
void exitWithMessage(string msg, string func_name, int line);

#endif /* debug_h_ */
