/*------------------------------------------------------------------------------
 * File: fileio.h
 * Created: June 11, 2015
 * Last changed: June 20, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Functions for doing file I/O operations.
 *----------------------------------------------------------------------------*/

#ifndef fileio_h_
#define fileio_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"

#include <stdint.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

const string* ioFileExt(const string* file_name);

/*--------------------------------------
 * Function: ioFileSize(file_name)
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
 *   long num_bytes = ioFileSize("foo.bin");
 *------------------------------------*/
long ioFileSize(const string* file_name);

/*--------------------------------------
 * Function: ioReadFile(file_name)
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
 *   uint8_t* data = ioReadFile("foo.bin");
 *------------------------------------*/
uint8_t* ioReadFile(const string* file_name);

#endif // fileio_h_
