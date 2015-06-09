/*------------------------------------------------------------------------------
 * File: sa14-game1.c
 * Created: June 7, 2015
 * Last changed: June 7, 2015
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

#include "ui/window.h"

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
static void printIntroMessage() {
    printf("sa14-game1 v%s by %s\n\n\n", PROGRAM_VERSION, PROGRAM_AUTHORS);
}

/*--------------------------------------
 * Function: main()
 *
 * Description:
 *   Programmets huvudfunktion.
 *------------------------------------*/
main() {
    printIntroMessage();

    windowADT window = createWindow("Main Window", 640, 480);

    while (isWindowOpen(window)) {
        printf("%d\t%d\n", getMouseX(window), getMouseY(window));
        updateWindow(window);
    }

    destroyWindow(window);

    system("pause");
}
