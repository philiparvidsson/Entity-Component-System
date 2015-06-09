/*------------------------------------------------------------------------------
 * File: array.c
 * Created: June 8, 2015
 * Last changed: June 8, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder en dynamisk array-typ.
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "array.h"

#include <stdlib.h>
#include <string.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: INITIAL_MAX_ELEMS
 *
 * Description:
 *   Det initiala maximala antalet element i en array.
 *------------------------------------*/
#define INITIAL_MAX_ELEMS 8 /* �tta �r nog lagom. */

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: arrayCDT
 *
 * Description:
 *   Den konkreta typen som anv�nds f�r att implementera dynamiska arrayer.
 *------------------------------------*/
struct arrayCDT {
    void *data;
    int num_elems;
    int max_elems;
    size_t elem_size;
};

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: doubleArrayCapacity()
 * Parameters:
 *   a  Den array vars kapacitet ska f�rdubblas.
 *
 *
 * Description:
 *   Dubblar kapaciteten f�r den specificerade arrayen.
 *------------------------------------*/
static void doubleArrayCapacity(arrayADT a) {
    /*
     * Vi dubblar kapaciteten och kopierar �ver de gamla elementen till den nya
     * minnesplatsen, sen sl�pper vi den gamla arrayen ur minnet.
     */

    void *old_data         = a->data;
    int   old_max_elems = a->max_elems;

    a->max_elems *= 2;
    a->data       = malloc(a->elem_size * a->max_elems);

    memcpy(a->data, old_data, a->elem_size * old_max_elems);

    free(old_data);
}

/*--------------------------------------
 * Function: arrayAdd()
 * Parameters:
 *   array  Arrayen till vilken ett element ska l�ggas.
 *   data   En pekare till elementdatan.
 *
 * Returns:
 *   En pekare till minnesplatsen dit elementet kopierades.
 *
 * Description:
 *   L�gger till ett element i den specificerade arrayen.
 *------------------------------------*/
void *arrayAdd(arrayADT array, const void *data) {
    /* Om det �r fullt s� g�r vi helt enkelt utrymme f�r fler element. */
    if (array->num_elems >= array->max_elems)
        doubleArrayCapacity(array);

    void *dest = (char *)array->data + array->num_elems * array->elem_size;
    
    memcpy(dest, data, array->elem_size);
    array->num_elems++;

    return dest;
}

/*--------------------------------------
 * Function: arrayLength()
 * Parameters:
 *   array  Arrayen som anropet g�ller.
 *
 * Returns:
 *   Det antal element som den specificerade arrayen inneh�ller.
 *
 * Description:
 *   Returnerar l�ngden, i antal element, p� den specificerade arrayen.
 *------------------------------------*/
int arrayLength(arrayADT array) {
    return array->num_elems;
}

/*--------------------------------------
 * Function: freeArray()
 * Parameters:
 *   array  Arrayen som ska deallokeras.
 *
 * Description:
 *   Deallokerar en array.
 *------------------------------------*/
void freeArray(arrayADT array) {
    free(array->data);
    free(array);
}

/*--------------------------------------
 * Function: newArray()
 * Parameters:
 *   elem_size  Storleken p� elementen i arrayen.
 *
 * Returns:
 *   En pekare till arrayen.
 *
 * Description:
 *   Skapar en ny, dynamisk array.
 *------------------------------------*/
arrayADT newArray(size_t elem_size) {
    arrayADT array = malloc(sizeof(struct arrayCDT));

    array->data      = malloc(elem_size * INITIAL_MAX_ELEMS);
    array->num_elems = 0;
    array->max_elems = INITIAL_MAX_ELEMS;
    array->elem_size = elem_size;

    return (array);
}
