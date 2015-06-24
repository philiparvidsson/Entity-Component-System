/*------------------------------------------------------------------------------
 * File: file_io.c
 * Created: June 11, 2015
 * Last changed: June 20, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Functions for doing file I/O operations.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "file_io.h"

#include "core/common.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: fileSize()
 * Parameters:
 *   file_name  The name of the file to retrieve the file size for.
 *
 * Returns:
 *   The size of the file with the specified name, in bytes.
 *
 * Description:
 *   Returns the size, in bytes, of the file with the specified name.
 *
 * Usage:
 *   long num_bytes = fileSize("foo.bin");
 *------------------------------------*/
long fileSize(const string* file_name) {
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
 *   file_name  The name of the file to read into a buffer.
 *
 * Returns:
 *   A pointer to the newly allocated buffer, containing the file data.
 *
 * Description:
 *   Allocates a buffer and reads all data from the specified file into it. A
 *   null-char is always appended at the end of the buffer, making the buffer
 *   safe to use as a string when reading text files. Do not forget to call the
 *   free() function on the returned pointer.
 *
 * Usage:
 *   uint8_t *data = readFile("foo.bin");
 *------------------------------------*/
uint8_t *readFile(const string* file_name) {
    FILE* fp = fopen(file_name, "rb");

    if (!fp)
        return (NULL);

    fseek(fp, 0, SEEK_END);
    long num_bytes = ftell(fp);
    rewind(fp);

    uint8_t *s = malloc(num_bytes+1);

    if (s) {
        fread(s, sizeof(char), num_bytes, fp);
        s[num_bytes] = '\0';
    }

    fclose(fp);

    return (s);
}
