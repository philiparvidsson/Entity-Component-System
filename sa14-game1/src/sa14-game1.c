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

#include <stdlib.h>
#include <stdio.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: PrintIntroMessage()
 * Parameters:
 *
 * Description:
 *   Skriver ut introduktionsmeddelandet.
 *------------------------------------*/
void PrintIntroMessage() {
    printf("sa14-game1 v%s by %s\n\n\n", PROGRAM_VERSION, PROGRAM_AUTHORS);
}

/*--------------------------------------
 * Function: main()
 * Parameters:
 *
 * Description:
 *   Programmets huvudfunktion.
 *------------------------------------*/
main() {
    PrintIntroMessage();
    system("pause");
}
