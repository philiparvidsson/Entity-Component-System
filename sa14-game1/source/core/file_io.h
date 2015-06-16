/*------------------------------------------------------------------------------
 * File: file_io.h
 * Created: June 11, 2015
 * Last changed: June 16, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Functions for doing file I/O operations.
 *----------------------------------------------------------------------------*/

#ifndef file_io_h_
#define file_io_h_

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
 *   file_name  Namnet p� den fil vars storlek ska l�sas ut.
 *
 * Returns:
 *   Storleken p� filen med det specificerade namnet, i antal bytes.
 *
 * Description:
 *   Returnerar storleken p� den specificerade filen, i antal bytes.
 *------------------------------------*/
long fileSize(string file_name);

/*--------------------------------------
 * Function: readFile()
 * Parameters:
 *   file_name  Namnet p� den fil som ska l�sas in till en str�ng.
 *
 * Returns:
 *   En pekare till det minnesblock dit filens inneh�ll l�sts in.
 *
 * Description:
 *   L�ser in en fil till en str�ng. Gl�m inte att anropa free() p� pekaren.
 *   Funktionen l�gger till en null-char i slutet.
 *------------------------------------*/
string readFile(string file_name);

#endif // file_io_h_
