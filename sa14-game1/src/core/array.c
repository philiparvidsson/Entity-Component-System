/*------------------------------------------------------------------------------
 * File: array.c
 * Created: June 8, 2015
 * Last changed: June 9, 2015
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
#define INITIAL_MAX_ELEMS (8) /* �tta �r nog lagom. */

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: arrayT_
 *
 * Description:
 *   Representerar en array. Denna datatyp anv�nds internt, men exponrerar �ven
 *   en publik del.
 *------------------------------------*/
typedef struct {
    /* --- Public --- */

    size_t elem_size; /* Storleken p� elementen i arrayen, i bytes. */
    int    num_elems; /* Antal element i arrayen. */

    /* --- Private --- */

    int    max_elems; /* Arrayens nuvarande kapacitet. */
    void  *data;      /* Datablock d�r elementen lagras. */
} arrayT_;

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
static void doubleArrayCapacity(arrayT_ *a) {
    /*
     * Vi dubblar kapaciteten och kopierar �ver de gamla elementen till den nya
     * minnesplatsen, sen sl�pper vi den gamla arrayen ur minnet.
     */

    void *old_data      = a->data;
    int   old_max_elems = a->max_elems;

    a->max_elems *= 2;
    a->data       = malloc(a->elem_size * a->max_elems);

    memcpy(a->data, old_data, a->elem_size * old_max_elems);

    free(old_data);
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
arrayT *newArray(size_t elem_size) {
    arrayT_ *a = malloc(sizeof(arrayT_));

    a->data      = malloc(elem_size * INITIAL_MAX_ELEMS);
    a->num_elems = 0;
    a->max_elems = INITIAL_MAX_ELEMS;
    a->elem_size = elem_size;

    return ((arrayT *)a);
}


/*--------------------------------------
 * Function: freeArray()
 * Parameters:
 *   a  Arrayen som ska deallokeras.
 *
 * Description:
 *   Deallokerar en array.
 *------------------------------------*/
void freeArray(arrayT *a) {
    free(((arrayT_ *)a)->data);
    free(a);
}


/*--------------------------------------
 * Function: arrayAdd()
 * Parameters:
 *   a     Arrayen till vilken ett element ska l�ggas.
 *   data  En pekare till elementdatan.
 *
 * Returns:
 *   En pekare till minnesplatsen dit elementet kopierades.
 *
 * Description:
 *   L�gger till ett element i den specificerade arrayen.
 *------------------------------------*/
void *arrayAdd(arrayT *a, const void *data) {
    /* Om det �r fullt s� g�r vi helt enkelt utrymme f�r fler element. */
    if (a->num_elems >= ((arrayT_ *)a)->max_elems)
        doubleArrayCapacity((arrayT_ *)a);

    void *dest = (char *)((arrayT_ *)a)->data + a->num_elems * a->elem_size;
    
    memcpy(dest, data, a->elem_size);
    ((arrayT_ *)a)->num_elems++;

    return dest;
}
