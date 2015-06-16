/*------------------------------------------------------------------------------
 * File: common.h
 * Created: June 7, 2015
 * Last changed: June 16, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Macros, constants, types, functions etc. that are common to many parts of
 *   the program.
 *
 *----------------------------------------------------------------------------*/

#ifndef common_h_
#define common_h_

/*------------------------------------------------
 * CONFIGURATION
 *----------------------------------------------*/

/*
 * If we're on MSVC++ we can configure the compilation and linking processes
 * here.
 *
 * @To-do: Move out to a makefile or such.
 */
#ifdef _MSC_VER

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

/*
 * This warning is not quite relevant since anonymous structs and unions are
 * allowed in the C11 standard, but some compilers are not quite there yet and
 * consider it nonstandard.
 *
 * warning C4201: 'nonstandard extension used : nameless struct/union'
 */
#pragma warning(disable:4201)

/*
 * We don't need to see warnings about functions being inlined.
 *
 * warning C4711: 'function selected for automatic inline expansion'
 */
#pragma warning(disable:4711)

#define inline __inline

#endif // _MSC_VER

/*------------------------------------------------
 * MACROS
 *----------------------------------------------*/

/*--------------------------------------
 * Macro: string
 *
 * Description:
 *   String type macro.
 *------------------------------------*/
#define string stringT

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: ProgramAuthors
 *
 * Description:
 *   A string containing the program authors.
 *------------------------------------*/
#define ProgramAuthors ("Philip Arvidsson (philip@philiparvidsson.com)")

/*--------------------------------------
 * Constant: ProgramVersion
 *
 * Description:
 *   The program version.
 *------------------------------------*/
#define ProgramVersion ("0.01b")

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: stringT
 *
 * Description:
 *   String type definition.
 *------------------------------------*/
typedef char *stringT;

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

// Some includes might depend on definitions in this file, so we put them at the
// end of this file instead of the beginning.

#include "core/debug.h"

#include <stdbool.h>

#endif // common_h_
