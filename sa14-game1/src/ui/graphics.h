/*------------------------------------------------------------------------------
 * File: graphics.h
 * Created: June 9, 2015
 * Last changed: June 9, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder grafikfunktioner.
 *
 *----------------------------------------------------------------------------*/

#ifndef graphics_h_
#define graphics_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "ui/window.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: graphicsT
 *
 * Description:
 *   Typ som representerar ett grafik f�r ett visst f�nster.
 *------------------------------------*/
typedef struct {
    const windowT *window; /* F�nstret som grafikobjektet �r initierat f�r. */
} graphicsT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: initGraphics()
 * Parameters:
 *   window  Det f�nster som grafikobjektet ska initieras f�r.
 *
 * Returns:
 *   En pekare till grafikobjektet.
 *
 * Description:
 *   Initierar grafik f�r det specificerade f�nstret.
 *------------------------------------*/
graphicsT *initGraphics(windowT *window);

/*--------------------------------------
 * Function: freeGraphics()
 * Parameters:
 *   g  Det grafikobjekt som ska frias.
 *
 * Description:
 *   Frig�r det specificerade grafikobjektet.
 *------------------------------------*/
void freeGraphics(graphicsT *g);

/*--------------------------------------
 * Function: clearCanvas()
 * Parameters:
 *   g      Det grafikobjekt som anropet g�ller.
 *   red    R�d f�rgkomponent.
 *   green  Gr�n f�rgkomponent.
 *   blue   Bl� f�rgkomponent.
 *
 * Description:
 *   Rensar ritytan til den specificerade f�rgen.
 *------------------------------------*/
void clearCanvas(graphicsT *g, float red, float green, float blue);

/*--------------------------------------
 * Function: setColor()
 * Parameters:
 *   g      Det grafikobjekt som anropet g�ller.
 *   red    R�d f�rgkomponent.
 *   green  Gr�n f�rgkomponent.
 *   blue   Bl� f�rgkomponent.
 *
 * Description:
 *   �ndrar f�rg f�r n�stkommande anrop till ritfunktioner.
 *------------------------------------*/
void setColor(graphicsT *g, float red, float green, float blue);

/*--------------------------------------
 * Function: updateGraphics()
 * Parameters:
 *   g      Det grafikobjekt som anropet g�ller.
 *
 * Description:
 *   Uppdaterar grafiken och ritar upp den i det associerade f�nstret.
 *------------------------------------*/
void updateGraphics(graphicsT *g);

#endif /* graphics_h_ */
