/*------------------------------------------------------------------------------
 * File: array.h
 * Created: June 8, 2015
 * Last changed: June 20, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Provides a dynamic array type.
 *----------------------------------------------------------------------------*/

#ifndef array_h_
#define array_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"

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
 * Function: arrayClear(a)
 * Parameters:
 *   a  The array to clear.
 *
 * Description:
 *   Clears the specified array.
 *
 * Usage:
 *   arrayClear(my_array);
 *------------------------------------*/
void arrayClear(arrayT* a);

/*--------------------------------------
 * Function: arrayNew(elem_size)
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
 *   arrayT* int_array = arrayNew(sizeof(int));
 *------------------------------------*/
arrayT* arrayNew(size_t elem_size);

/*--------------------------------------
 * Function: arrayFree()
 * Parameters:
 *   a  The array to free from memory.
 *
 * Description:
 *   Frees the specified array from memory.
 *
 * Usage:
 *   arrayFree(my_array);
 *------------------------------------*/
void arrayFree(arrayT* a);

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
void* arrayAdd(arrayT* a, const void* elem);

/*--------------------------------------
 * Function: arrayRemove(a, i)
 * Parameters:
 *   a  The array to remove an element from.
 *   i  The index of the element to remove.
 *
 * Description:
 *   Removes an element from an array.
 *
 * Usage:
 *   arrayRemove(a, 0);
 *------------------------------------*/
void arrayRemove(arrayT* a, int i);

/*--------------------------------------
 * Function: arrayGet()
 * Parameters:
 *   a  The array to retrieve an element from.
 *   i  The index of the element to retrieve.
 *
 * Returns:
 *   A pointer to the element inside the array.
 *
 * Description:
 *   Retrieves a pointer to an element inside the array.
 *
 * Usage:
 *   myTypeT* ptr = (myTypeT*)arrayGet(my_array, 1);
 *------------------------------------*/
void* arrayGet(const arrayT* a, int i);

/*--------------------------------------
 * Function: arrayLength()
 * Parameters:
 *   a  The array to get the length of.
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
int arrayLength(const arrayT* a);

#endif // array_h_
