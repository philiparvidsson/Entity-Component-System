/*------------------------------------------------------------------------------
 * File: io.c
 * Created: June 11, 2015
 * Last changed: June 13, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder funktioner f�r att l�sa fr�n och skriva till filer m.m.
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "io.h"

#include "core/common.h"

#include <stdio.h>
#include <stdlib.h>

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
long fileSize(string file_name) {
    FILE* fp = fopen(file_name, "rb");

    if (!fp)
        return (-1);

    long num_bytes = ftell(fp);

    fclose(fp);

    return (num_bytes);
}

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
string readFile(string file_name) {
    FILE* fp = fopen(file_name, "rb");

    if (!fp)
        return (NULL);

    fseek(fp, 0, SEEK_END);
    long num_bytes = ftell(fp);
    rewind(fp);

    string s = malloc(num_bytes+1);

    if (s) {
        fread(s, sizeof(char), num_bytes, fp);
        s[num_bytes] = '\0';
    }

    fclose(fp);

    return (s);
}
