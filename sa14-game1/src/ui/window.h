/*------------------------------------------------------------------------------
 * File: window.h
 * Created: June 8, 2015
 * Last changed: June 9, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner för att skapa fönster.
 *
 *----------------------------------------------------------------------------*/

#ifndef window_h_
#define window_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: windowT
 *
 * Description:
 *   Datatyp som representerar ett fönster.
 *------------------------------------*/
typedef struct windowT {
    const boolT    is_open; /* Indikerar om fönstret stängts av användaren. */
    const int      width,   /* Fönstrets bredd i antal pixlar.              */
                   height;  /* Fönstrets höjd i antal pixlar.               */
    const stringT  title;   /* Fönstrets titel.                             */
} windowT;

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
windowT *createWindow(stringT title, int width, int height);

/*--------------------------------------
 * Function: destroyWindow()
 * Parameters:
 *   window  Fönstret som ska stängas och förstöras.
 *
 * Description:
 *   Stänger och förstör det specificerade fönstret.
 *------------------------------------*/
void destroyWindow(windowT *window);

/*--------------------------------------
 * Function: updateWindow()
 * Parameters:
 *   window  Fönstret som ska uppdateras.
 *
 * Description:
 *   Uppdaterar det specificerade fönstret.
 *------------------------------------*/
void updateWindow(windowT *window);

#endif /* window_h_ */
