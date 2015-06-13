/*------------------------------------------------------------------------------
 * File: array.c
 * Created: June 8, 2015
 * Last changed: June 13, 2015
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
 *   Det initiala maximala antalet element i en array.
 *------------------------------------*/
#define InitialCapacity (8) /* Åtta är nog lagom. */

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: arrayT_
 *
 * Description:
 *   Representerar en array. Denna datatyp används internt, men exponrerar även
 *   en publik del.
 *------------------------------------*/
typedef struct {
    /* --- Public --- */

    size_t elem_size; /* Storleken på elementen i arrayen, i bytes. */
    int    num_elems; /* Antal element i arrayen. */

    /* --- Private --- */

    int    max_elems; /* Arrayens nuvarande kapacitet. */
    void  *data;      /* Datablock där elementen lagras. */
} arrayT_;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*------------------------------------------------------------------------------
 * Privata funktioner.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: doubleArrayCapacity()
 * Parameters:
 *   a  Den array vars kapacitet ska fördubblas.
 *
 *
 * Description:
 *   Dubblar kapaciteten för den specificerade arrayen.
 *------------------------------------*/
static void doubleArrayCapacity(arrayT_ *a) {
    /*
     * Vi dubblar kapaciteten och kopierar över de gamla elementen till den nya
     * minnesplatsen, sen släpper vi den gamla arrayen ur minnet.
     */

    void *old_data      = a->data;
    int   old_max_elems = a->max_elems;

    a->max_elems *= 2;
    a->data       = malloc(a->elem_size * a->max_elems);

    memcpy(a->data, old_data, a->elem_size * old_max_elems);

    free(old_data);
}

/*------------------------------------------------------------------------------
 * Publika funktioner.
 *----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: newArray()
 * Parameters:
 *   elem_size  Storleken på elementen i arrayen.
 *
 * Returns:
 *   En pekare till arrayen.
 *
 * Description:
 *   Skapar en ny, dynamisk array.
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
 *   a     Arrayen till vilken ett element ska läggas.
 *   elem  En pekare till elementdatan.
 *
 * Returns:
 *   En pekare till minnesplatsen dit elementet kopierades.
 *
 * Description:
 *   Lägger till ett element i den specificerade arrayen.
 *------------------------------------*/
void *arrayAdd(arrayT *a, const void *elem) {
    /* Om det är fullt så gör vi helt enkelt utrymme för fler element. */
    if (a->num_elems >= ((arrayT_ *)a)->max_elems)
        doubleArrayCapacity((arrayT_ *)a);

    void *dest = (char *)((arrayT_ *)a)->data + (a->num_elems * a->elem_size);
    
    memcpy(dest, elem, a->elem_size);
    ((arrayT_ *)a)->num_elems++;

    return (dest);
}
/*--------------------------------------
 * Function: arrayGet()
 * Parameters:
 *   a     Arrayen från vilket ett element ska läsas ut.
 *   i     Index till det element som ska läsas ut.
 *   elem  Pekare till datablock där elementat ska lagras.
 *
 * Description:
 *   Läser ut ett element från arrayen.
 *------------------------------------*/
void arrayGet(const arrayT *a, int i, void *dest) {
    assert(0 <= i && i < a->num_elems);

    void *src = ((char *)((arrayT_ *)a)->data + (i * a->elem_size));

    memcpy(dest, src, a->elem_size);
}
