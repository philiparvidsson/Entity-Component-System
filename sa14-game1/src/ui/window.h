/*------------------------------------------------------------------------------
 * File: window.h
 * Created: June 8, 2015
 * Last changed: June 8, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner f�r att skapa f�nster.
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
 *   Abstrakt pekartyp som representerar ett f�nster.
 *------------------------------------*/
typedef struct windowCDT *windowADT;

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
windowADT createWindow(stringT title, int width, int height);

/*--------------------------------------
 * Function: destroyWindow()
 * Parameters:
 *   window  F�nstret som ska st�ngas och f�rst�ras.
 *
 * Description:
 *   St�nger och f�rst�r det specificerade f�nstret.
 *------------------------------------*/
void destroyWindow(windowADT window);

/*--------------------------------------
 * Function: updateWindow()
 * Parameters:
 *   window  F�nstret som ska uppdateras.
 *
 * Description:
 *   Uppdaterar det specificerade f�nstret.
 *------------------------------------*/
void updateWindow(windowADT window);

#endif // _window_h_
