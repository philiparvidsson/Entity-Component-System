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
 *   Typ som representerar ett grafik för ett visst fönster.
 *------------------------------------*/
typedef struct {
    const windowT *window; /* Fönstret som grafikobjektet är initierat för. */
} graphicsT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: initGraphics()
 * Parameters:
 *   window  Det fönster som grafikobjektet ska initieras för.
 *
 * Returns:
 *   En pekare till grafikobjektet.
 *
 * Description:
 *   Initierar grafik för det specificerade fönstret.
 *------------------------------------*/
graphicsT *initGraphics(windowT *window);

/*--------------------------------------
 * Function: freeGraphics()
 * Parameters:
 *   g  Det grafikobjekt som ska frias.
 *
 * Description:
 *   Frigör det specificerade grafikobjektet.
 *------------------------------------*/
void freeGraphics(graphicsT *g);

/*--------------------------------------
 * Function: clearCanvas()
 * Parameters:
 *   g      Det grafikobjekt som anropet gäller.
 *   red    Röd färgkomponent.
 *   green  Grön färgkomponent.
 *   blue   Blå färgkomponent.
 *
 * Description:
 *   Rensar ritytan til den specificerade färgen.
 *------------------------------------*/
void clearCanvas(graphicsT *g, float red, float green, float blue);

/*--------------------------------------
 * Function: setColor()
 * Parameters:
 *   g      Det grafikobjekt som anropet gäller.
 *   red    Röd färgkomponent.
 *   green  Grön färgkomponent.
 *   blue   Blå färgkomponent.
 *
 * Description:
 *   Ändrar färg för nästkommande anrop till ritfunktioner.
 *------------------------------------*/
void setColor(graphicsT *g, float red, float green, float blue);

/*--------------------------------------
 * Function: updateGraphics()
 * Parameters:
 *   g      Det grafikobjekt som anropet gäller.
 *
 * Description:
 *   Uppdaterar grafiken och ritar upp den i det associerade fönstret.
 *------------------------------------*/
void updateGraphics(graphicsT *g);

#endif /* graphics_h_ */
