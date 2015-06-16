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
long fileSize(string file_name);

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
 *   string data = readFile("foo.bin");
 *------------------------------------*/
string readFile(string file_name);

#endif // file_io_h_
