/*------------------------------------------------------------------------------
 * File: common.h
 * Created: June 7, 2015
 * Last changed: June 7, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Blandade saker som behövs här och där.
 *
 *----------------------------------------------------------------------------*/

#ifndef _common_h_
#define _common_h_

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: PROGRAM_AUTHORS
 *
 * Description:
 *   De programmerare som varit med och programmerat programmet.
 *------------------------------------*/
#define PROGRAM_AUTHORS "Philip Arvidsson (philip@philiparvidsson.com)"

/*--------------------------------------
 * Constant: PROGRAM_VERSION
 *
 * Description:
 *   Programmets version.
 *------------------------------------*/
#define PROGRAM_VERSION "0.01b"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: bool
 *
 * Description:
 *   Boolesk typ.
 *------------------------------------*/
typedef enum { FALSE, TRUE } bool;

/*--------------------------------------
 * Type: pointT
 *
 * Description:
 *   Representerar en punkt i 2-dimensionell rymd.
 *------------------------------------*/
typedef struct {
    float x, y;
} pointT;

#endif // _common_h_
