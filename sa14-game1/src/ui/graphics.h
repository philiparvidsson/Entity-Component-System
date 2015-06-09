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

#ifndef graphics_h__
#define graphics_h__

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
    const windowT window; /* Det fönster som grafikobjektet är initierat för. */
} *graphicsT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

graphicsT initGraphics(windowT window);
void freeGraphics(graphicsT g);

void clearCanvas(graphicsT g, float red, float green, float blue);

void setColor(graphicsT g, float red, float green, float blue);

void swapBuffers(graphicsT g);

#endif /* graphics_h__ */
