#ifndef text_h_
#define text_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void drawText(const string* text, float x, float y, const string* font_name, int font_size);
void loadFontFromFile(const string* file_name);
void loadFontFromMemory(void* font_data, int num_bytes);

#endif // text_h_
