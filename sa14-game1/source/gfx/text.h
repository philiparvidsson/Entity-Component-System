#ifndef text_h_
#define text_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void drawText(string const *text, int point_size);
void loadFontFromFile(string const *file_name);

#endif // text_h_
