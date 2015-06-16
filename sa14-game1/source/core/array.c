/*------------------------------------------------------------------------------
 * File: array.c
 * Created: June 8, 2015
 * Last changed: June 16, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Provides a dynamic array type.
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "array.h"

#include "core/common.h"

#include <stdlib.h>
#include <string.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: InitialCapacity
 *
 * Description:
 *   The initial array capacity, in number of elements.
 *------------------------------------*/
#define InitialCapacity (8) /* Eight is probably enough. */

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: arrayT_
 *
 * Description:
 *   Represents an array of elements. This type is used internally but also
 *   exposes a public interface to clients (see the typ arrayT in the header
 *   file).
 *------------------------------------*/
typedef struct {
    /* --- Public --- */

    size_t elem_size; // The size of each array element, in bytes.
    int    num_elems; // Number of elements in the array.

    /* --- Private --- */

    int    max_elems; // The current array capacity.
    void  *data;      // The element data block.
} arrayT_;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*------------------------------------------------------------------------------
 * Private functions.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: doubleArrayCapacity()
 * Parameters:
 *   a  The array whose capacity should be doubled.
 *
 *
 * Description:
 *   Doubles the capacity for the specified array by reallocating its data
 *   block twice as big as it currently is.
 *
 * Usage:
 *   doubleArrayCapacity(my_array);
 *------------------------------------*/
static void doubleArrayCapacity(arrayT_ *a) {
    // We double the array capacity and copy the old elements into the new
    // data block, the release the old data block.

    void *old_data      = a->data;
    int   old_max_elems = a->max_elems;

    a->max_elems *= 2;
    a->data       = malloc(a->elem_size * a->max_elems);

    memcpy(a->data, old_data, a->elem_size * old_max_elems);

    free(old_data);
}

/*------------------------------------------------------------------------------
 * Public functions.
 *----------------------------------------------------------------------------*/

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
arrayT *newArray(size_t elem_size) {
    arrayT_ *a = malloc(sizeof(arrayT_));

    a->data      = malloc(elem_size * InitialCapacity);
    a->num_elems = 0;
    a->max_elems = InitialCapacity;
    a->elem_size = elem_size;

    return ((arrayT *)a);
}

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
void freeArray(arrayT *a) {
    free(((arrayT_ *)a)->data);
    free(a);
}

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
void arrayAdd(arrayT *a, const void *elem) {
    // If the array is full, we double its capacity.
    if (a->num_elems >= ((arrayT_ *)a)->max_elems)
        doubleArrayCapacity((arrayT_ *)a);

    void *dest = (char *)((arrayT_ *)a)->data + (a->num_elems * a->elem_size);
    
    memcpy(dest, elem, a->elem_size);
    ((arrayT_ *)a)->num_elems++;
}

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
void arrayGet(const arrayT *a, int i, void *dest) {
    assert(0 <= i && i < a->num_elems);

    void *src = ((char *)((arrayT_ *)a)->data + (i * a->elem_size));
    memcpy(dest, src, a->elem_size);
}
