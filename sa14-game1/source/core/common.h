/*------------------------------------------------------------------------------
 * File: common.h
 * Created: June 7, 2015
 * Last changed: June 13, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Blandade saker som beh�vs h�r och d�r.
 *
 *----------------------------------------------------------------------------*/

#ifndef common_h_
#define common_h_

/*------------------------------------------------
 * CONFIGURATION
 *----------------------------------------------*/

/*
 * H�r konfigurerar vi kompileringen och l�nkningen en del. I alla fall om vi
 * anv�nder Microsoft Visual C++-kompilatorn.
 */
#ifdef _MSC_VER

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

/* warning C4711: 'function selected for automatic inline expansion' */
#pragma warning(disable:4711)

#endif /* _MSC_VER */

/*------------------------------------------------
 * MACROS
 *----------------------------------------------*/

/*--------------------------------------
 * Macro: string
 *
 * Description:
 *   Makro f�r str�ngtyp.
 *------------------------------------*/
#define string stringT

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: ProgramAuthors
 *
 * Description:
 *   De programmerare som varit med och programmerat programmet.
 *------------------------------------*/
#define ProgramAuthors ("Philip Arvidsson (philip@philiparvidsson.com)")

/*--------------------------------------
 * Constant: ProgramVersion
 *
 * Description:
 *   Programmets version.
 *------------------------------------*/
#define ProgramVersion ("0.01b")

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: stringT
 *
 * Description:
 *   Str�ngtyp.
 *------------------------------------*/
typedef char *stringT;

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/debug.h"

#include <stdbool.h>

#endif /* common_h_ */
