/*------------------------------------------------------------------------------
 * File: array.h
 * Created: June 8, 2015
 * Last changed: June 20, 2015
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
typedef struct arrayT arrayT;

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
 * Returns:
 *   A pointer to the element inside the array.
 *
 * Description:
 *   Adds an element to the end of the specified array.
 *
 * Usage:
 *   arrayAdd(my_array, &elem);
 *------------------------------------*/
void *arrayAdd(arrayT *a, const void *elem);

/*--------------------------------------
 * Function: arrayGet()
 * Parameters:
 *   a     The array to retrieve an element from.
 *   i     The index of the element to retrieve.
 *
 * Returns:
 *   A pointer to the element inside the array.
 *
 * Description:
 *   Retrieves a pointer to an element inside the array.
 *
 * Usage:
 *   myTypeT *ptr = (myTypeT *)arrayGet(my_array, 1);
 *------------------------------------*/
void *arrayGet(const arrayT *a, int i);

/*--------------------------------------
 * Function: arrayLength()
 * Parameters:
 *   a     The array to get the length of.
 *
 * Returns:
 *   The number of elements in the specified array.
 *
 * Description:
 *   Gets the number of elements in an array.
 *
 * Usage:
 *   int num_elements = arrayLength(my_array);
 *------------------------------------*/
int arrayLength(const arrayT *a);

void arrayRemove(arrayT *a, int i);
void arrayRemoveElem(arrayT *a, void *elem);

#endif // array_h_
