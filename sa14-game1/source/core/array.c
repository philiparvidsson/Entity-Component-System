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
#define InitialCapacity (8) /* �tta �r nog lagom. */

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

/*------------------------------------------------------------------------------
 * Privata funktioner.
 *----------------------------------------------------------------------------*/

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

/*------------------------------------------------------------------------------
 * Publika funktioner.
 *----------------------------------------------------------------------------*/

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
 *   a     Arrayen till vilken ett element ska l�ggas.
 *   elem  En pekare till elementdatan.
 *
 * Returns:
 *   En pekare till minnesplatsen dit elementet kopierades.
 *
 * Description:
 *   L�gger till ett element i den specificerade arrayen.
 *------------------------------------*/
void *arrayAdd(arrayT *a, const void *elem) {
    /* Om det �r fullt s� g�r vi helt enkelt utrymme f�r fler element. */
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
 *   a     Arrayen fr�n vilket ett element ska l�sas ut.
 *   i     Index till det element som ska l�sas ut.
 *   elem  Pekare till datablock d�r elementat ska lagras.
 *
 * Description:
 *   L�ser ut ett element fr�n arrayen.
 *------------------------------------*/
void arrayGet(const arrayT *a, int i, void *dest) {
    assert(0 <= i && i < a->num_elems);

    void *src = ((char *)((arrayT_ *)a)->data + (i * a->elem_size));

    memcpy(dest, src, a->elem_size);
}
