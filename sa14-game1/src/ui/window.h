/*------------------------------------------------------------------------------
 * File: window.h
 * Created: June 8, 2015
 * Last changed: June 8, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner för att skapa fönster.
 *
 *----------------------------------------------------------------------------*/

#ifndef _window_h_
#define _window_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: windowADT
 *
 * Description:
 *   Abstrakt pekartyp som representerar ett fönster.
 *------------------------------------*/
typedef struct windowCDT *windowADT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: createWindow()
 * Parameters:
 *   title   Fönstrets titel.
 *   width   Fönstrets bredd, i antal pixlar.
 *   height  Fönstrets höjd, i antal pixlar.
 *
 * Returns:
 *   En pekare till fönstret.
 *
 * Description:
 *   Skapar ett fönster med de specificerade dimensionerna. Den angivna bredden
 *   och höjden inkluderar inte fönsterdekorationer, utan endast storleken på
 *   klientytan.
 *------------------------------------*/
windowADT createWindow(stringT title, int width, int height);

/*--------------------------------------
 * Function: destroyWindow()
 * Parameters:
 *   window  Fönstret som ska stängas och förstöras.
 *
 * Description:
 *   Stänger och förstör det specificerade fönstret.
 *------------------------------------*/
void destroyWindow(windowADT window);

/*--------------------------------------
 * Function: updateWindow()
 * Parameters:
 *   window  Fönstret som ska uppdateras.
 *
 * Description:
 *   Uppdaterar det specificerade fönstret.
 *------------------------------------*/
void updateWindow(windowADT window);

#endif // _window_h_
