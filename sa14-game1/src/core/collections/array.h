/*------------------------------------------------------------------------------
 * File: array.h
 * Created: June 8, 2015
 * Last changed: June 8, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder en dynamisk array-typ.
 *
 *----------------------------------------------------------------------------*/

#ifndef _array_h_
#define _array_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include <stdlib.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: arrayADT
 *
 * Description:
 *   Abstrakt pekartyp som representerar en array.
 *------------------------------------*/
typedef struct arrayCDT *arrayADT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

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
void *arrayAdd(arrayADT array, const void *data);

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
int arrayLength(arrayADT array);

/*--------------------------------------
 * Function: freeArray()
 * Parameters:
 *   array  Arrayen som ska deallokeras.
 *
 * Description:
 *   Deallokerar en array.
 *------------------------------------*/
void freeArray(arrayADT array);

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
arrayADT newArray(size_t element_size);

#endif /* _array_h_ */
