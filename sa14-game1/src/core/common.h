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

#ifndef common_h_
#define common_h_

/*------------------------------------------------
 * CONFIGURATION
 *----------------------------------------------*/

/*
 * Här konfigurerar vi kompileringen och länkningen en del. I alla fall om vi
 * använder Microsoft Visual C++-kompilatorn.
 */
#ifdef _MSC_VER

#pragma comment(lib, "opengl32.lib")

/* warning C4711: 'function selected for automatic inline expansion' */
#pragma warning(disable:4711)

#endif /* _MSC_VER */

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/debug.h"

#include <stdbool.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: PROGRAM_AUTHORS
 *
 * Description:
 *   De programmerare som varit med och programmerat programmet.
 *------------------------------------*/
#define PROGRAM_AUTHORS ("Philip Arvidsson (philip@philiparvidsson.com)")

/*--------------------------------------
 * Constant: PROGRAM_VERSION
 *
 * Description:
 *   Programmets version.
 *------------------------------------*/
#define PROGRAM_VERSION ("0.01b")

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

#endif /* common_h_ */
