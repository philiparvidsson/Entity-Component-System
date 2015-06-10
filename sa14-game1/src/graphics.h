/*------------------------------------------------------------------------------
 * File: graphics.h
 * Created: June 8, 2015
 * Last changed: June 10, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder ett enkelt grafikbibliotek för att rita och visa grafik. Biblio-
 *   teket använder OpenGL och renderar således med hårdvara om möjlighet finns.
 *
 *----------------------------------------------------------------------------*/

#ifndef graphics_h_
#define graphics_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*------------------------------------------------------------------------------
 * Funktioner för att initiera grafikläge, avsluta grafikläge samt konfigurera
 * grafikläget och ställa in parametrar.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: initGraphics()
 * Parameters:
 *   title   Fönstrets titel.
 *   width   Fönstrets bredd, i antal pixlar.
 *   height  Fönstrets höjd, i antal pixlar.
 *
 * Description:
 *   Skapar ett grafikfönster med de specificerade dimensionerna. Den angivna
 *   bredden och höjden inkluderar inte fönsterdekorationer, utan endast storleken
 *   storleken på klientytan.
 *------------------------------------*/
void initGraphics(const char *title, int width, int height);

/*--------------------------------------
 * Function: exitGraphics()
 * Parameters:
 *
 * Description:
 *   Stänger grafikfönstret.
 *------------------------------------*/
void exitGraphics(void);

/*--------------------------------------
 * Function: setFrameRate()
 * Parameters:
 *
 * Description:
 *   Ställer in hur många bildrutor som ska visas per sekund. Ange noll för
 *   obegränsad hastighet.
 *------------------------------------*/
void setFrameRate(float fps);

/*------------------------------------------------------------------------------
 * Funktioner för att rensa ritytan, rita geomtriska figurer och texturer samt
 * för att presentera ritytan på skärmen m.m.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: clearCanvas()
 * Parameters:
 *   r  Röd färgkomponent.
 *   g  Grön färgkomponent.
 *   b  Blå färgkomponent.
 *
 * Description:
 *   Rensar ritytan til den specificerade färgen.
 *------------------------------------*/
void clearCanvas(float r, float g, float b);

/*--------------------------------------
 * Function: setColor()
 * Parameters:
 *   r  Röd färgkomponent.
 *   g  Grön färgkomponent.
 *   b  Blå färgkomponent.
 *
 * Description:
 *   Ändrar färg för nästkommande anrop till ritfunktioner.
 *------------------------------------*/
void setColor(float r, float g, float b);

/*--------------------------------------
 * Function: updateDisplay()
 * Parameters:
 *
 * Description:
 *   Uppdaterar det grafikfönstret.
 *------------------------------------*/
void updateDisplay(void);

/*------------------------------------------------------------------------------
 * Funktioner för att läsa ut inställningar och information om grafikläget m.m.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: windowIsOpen()
 * Parameters:
 *
 * Returns:
 *   Sant om grafikfönstret är öppet, annars falskt.
 *
 * Description:
 *   Returnerar sant om grafikfönstret är öppet.
 *------------------------------------*/
boolT windowIsOpen(void);

#endif /* graphics_h_ */
