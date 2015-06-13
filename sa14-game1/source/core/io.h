/*------------------------------------------------------------------------------
 * File: io.h
 * Created: June 11, 2015
 * Last changed: June 13, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder funktioner för att läsa från och skriva till filer m.m.
 *
 *----------------------------------------------------------------------------*/

#ifndef io_h_
#define io_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: fileSize()
 * Parameters:
 *   file_name  Namnet på den fil vars storlek ska läsas ut.
 *
 * Returns:
 *   Storleken på filen med det specificerade namnet, i antal bytes.
 *
 * Description:
 *   Returnerar storleken på den specificerade filen, i antal bytes.
 *------------------------------------*/
long fileSize(string file_name);

/*--------------------------------------
 * Function: readFile()
 * Parameters:
 *   file_name  Namnet på den fil som ska läsas in till en sträng.
 *
 * Returns:
 *   En pekare till det minnesblock dit filens innehåll lästs in.
 *
 * Description:
 *   Läser in en fil till en sträng. Glöm inte att anropa free() på pekaren.
 *   Funktionen lägger till en null-char i slutet.
 *------------------------------------*/
string readFile(string file_name);

#endif
