#ifndef mouse_h_
#define mouse_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct mouseT mouseT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

mouseT* mouseAlloc(void);
void mouseInit(mouseT* mouse);
mouseT* mouseNew(void);
void mouseFree(mouseT* mouse);
void mouseUpdate(mouseT* mouse);

#endif // mouse_h_
