/*------------------------------------------------------------------------------
 * File: array.c
 * Created: June 8, 2015
 * Last changed: June 20, 2015
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
#include <string.h> // memcpy(), memmove()

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
 * Type: arrayT
 *
 * Description:
 *   Represents an array of elements.
 *------------------------------------*/
struct arrayT {
    void*  data;      // The element data block.
    int    num_elems; // Number of elements in the array.
    int    max_elems; // The current array capacity.
    size_t elem_size; // The size of each array element, in bytes.
};

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*------------------------------------------------------------------------------
 * Private functions.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: doubleArrayCapacity(a)
 * Parameters:
 *   a  The array whose capacity should be doubled.
 *
 * Description:
 *   Doubles the capacity for the specified array by reallocating its data
 *   block twice as big as it currently is.
 *
 * Usage:
 *   doubleArrayCapacity(my_array);
 *------------------------------------*/
static void doubleArrayCapacity(arrayT* a) {
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
 * Function: newArray(elem_size)
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
 *   arrayT* int_array = newArray(sizeof(int));
 *------------------------------------*/
arrayT* newArray(size_t elem_size) {
    arrayT* a = malloc(sizeof(arrayT));

    a->data      = malloc(elem_size * InitialCapacity);
    a->num_elems = 0;
    a->max_elems = InitialCapacity;
    a->elem_size = elem_size;

    return (a);
}

/*--------------------------------------
 * Function: freeArray(a)
 * Parameters:
 *   a  The array to free from memory.
 *
 * Description:
 *   Frees the specified array from memory.
 *
 * Usage:
 *   freeArray(my_array);
 *------------------------------------*/
void freeArray(arrayT* a) {
    free(a->data);
    free(a);
}

/*--------------------------------------
 * Function: arrayAdd(a, elem)
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
void *arrayAdd(arrayT* a, const void* elem) {
    // If the array is full, we double its capacity.
    if (a->num_elems >= a->max_elems)
        doubleArrayCapacity(a);

    void *dest = (char*)a->data + (a->num_elems * a->elem_size);
    
    memcpy(dest, elem, a->elem_size);
    a->num_elems++;

    return (dest);
}

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
void arrayRemove(arrayT* a, int i) {
    assert(0 <= i && i < a->num_elems);

    void *dest = (char*)a->data + (i * a->elem_size);
    void *src  = (char*)dest + a->elem_size;
    memmove(dest, src, (a->num_elems-i+1) * a->elem_size);
    a->num_elems--;
}

/*--------------------------------------
 * Function: arrayGet(a, i)
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
void *arrayGet(const arrayT* a, int i) {
    assert(0 <= i && i < a->num_elems);

    void *ptr = (char*)a->data + (i * a->elem_size);
    return (ptr);
}

/*--------------------------------------
 * Function: arrayLength(a)
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
int arrayLength(const arrayT* a) {
    return (a->num_elems);
}
