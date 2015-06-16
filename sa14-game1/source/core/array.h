/*------------------------------------------------------------------------------
 * File: array.h
 * Created: June 8, 2015
 * Last changed: June 16, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Provides a dynamic array type.
 *----------------------------------------------------------------------------*/

#ifndef array_h_
#define array_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

#include <stddef.h> // size_t

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: arrayT
 *
 * Description:
 *   Represents an array of elements.
 *------------------------------------*/
typedef struct {
    const size_t elem_size; // The size of each array element, in bytes.
    const int    num_elems; // Number of elements in the array.
} arrayT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: newArray()
 * Parameters:
 *   elem_size  The array element size, in bytes.
 *
 * Returns:
 *   A pointer to the array.
 *
 * Description:
 *   Creates a new, dynamic array.
 *
 * Usage:
 *   arrayT *int_array = newArray(sizeof(int));
 *------------------------------------*/
arrayT *newArray(size_t elem_size);

/*--------------------------------------
 * Function: freeArray()
 * Parameters:
 *   a  The array to free from memory.
 *
 * Description:
 *   Frees the specified array from memory.
 *
 * Usage:
 *   freeArray(my_array);
 *------------------------------------*/
void freeArray(arrayT *a);

/*--------------------------------------
 * Function: arrayAdd()
 * Parameters:
 *   a     The array to add an element to.
 *   elem  Pointer to the element.
 *
 * Description:
 *   Adds an element to the end of the specified array.
 *
 * Usage:
 *   arrayAdd(my_array, &elem);
 *------------------------------------*/
void arrayAdd(arrayT *a, const void *elem);

/*--------------------------------------
 * Function: arrayGet()
 * Parameters:
 *   a     The array to retrieve an element from.
 *   i     The index of the element to retrieve.
 *   elem  Pointer to a buffer which the element data will be copied to.
 *
 * Description:
 *   Retrieves an element from the array.
 *
 * Usage:
 *   void *buf = malloc(my_array->elem_size);
 *   arrayGet(my_array, 1, &buf);
 *------------------------------------*/
void arrayGet(const arrayT *a, int i, void *dest);

#endif // array_h_
