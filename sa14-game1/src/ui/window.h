/*------------------------------------------------------------------------------
 * File: window.h
 * Created: June 8, 2015
 * Last changed: June 9, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner f�r att skapa f�nster.
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
 *   Datatyp som representerar ett f�nster.
 *------------------------------------*/
typedef struct windowT {
    const boolT    is_open; /* Indikerar om f�nstret st�ngts av anv�ndaren. */
    const int      width,   /* F�nstrets bredd i antal pixlar.              */
                   height;  /* F�nstrets h�jd i antal pixlar.               */
    const stringT  title;   /* F�nstrets titel.                             */
} windowT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: createWindow()
 * Parameters:
 *   title   F�nstrets titel.
 *   width   F�nstrets bredd, i antal pixlar.
 *   height  F�nstrets h�jd, i antal pixlar.
 *
 * Returns:
 *   En pekare till f�nstret.
 *
 * Description:
 *   Skapar ett f�nster med de specificerade dimensionerna. Den angivna bredden
 *   och h�jden inkluderar inte f�nsterdekorationer, utan endast storleken p�
 *   klientytan.
 *------------------------------------*/
windowT *createWindow(stringT title, int width, int height);

/*--------------------------------------
 * Function: destroyWindow()
 * Parameters:
 *   window  F�nstret som ska st�ngas och f�rst�ras.
 *
 * Description:
 *   St�nger och f�rst�r det specificerade f�nstret.
 *------------------------------------*/
void destroyWindow(windowT *window);

/*--------------------------------------
 * Function: updateWindow()
 * Parameters:
 *   window  F�nstret som ska uppdateras.
 *
 * Description:
 *   Uppdaterar det specificerade f�nstret.
 *------------------------------------*/
void updateWindow(windowT *window);

#endif /* window_h_ */
