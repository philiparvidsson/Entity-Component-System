/*------------------------------------------------------------------------------
 * File: sa14-game1.c
 * Created: June 7, 2015
 * Last changed: June 9, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Huvudprogram för spelet. Vad nu det innebär. Det får tiden utvisa. :-)
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: printIntroMessage()
 *
 * Description:
 *   Skriver ut introduktionsmeddelandet.
 *------------------------------------*/
static void printIntroMessage(void) {
    printf("sa14-game1 v%s by %s\n\n\n", PROGRAM_VERSION, PROGRAM_AUTHORS);
    printf("Some time in the not too distant future, this program will be a\n"
           "really cool game instead of this crap that does nothing...\n");
}

/*--------------------------------------
 * Function: main()
 *
 * Description:
 *   Programmets huvudfunktion.
 *------------------------------------*/
int main(void) {
    printIntroMessage();

    initGraphics("Main Window", 640, 480);
    setFrameRate(60.0f);

    while (windowIsOpen()) {
        clearCanvas(0.0f, 0.0f, 0.25f);
        updateDisplay();
    }

    exitGraphics();

    system("pause");
}
