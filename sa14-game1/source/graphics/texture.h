#ifndef texture_h_
#define texture_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"

#define TexFormatBMP (1)

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct textureT textureT;

/*------------------------------------------------
 * FUNCTIOMS
 *----------------------------------------------*/

textureT* createTexture(int width, int height);
textureT* createTextureFromScreen(void);
textureT* createWhiteTexture(void);
textureT* loadTextureFromFile(const void* file_name);
textureT* loadTextureFromMemory(const void* data, int format);
textureT* useTexture(textureT* texture, int index);
void freeTexture(textureT* texture);
bool getTextureRepeat(const textureT* texture);
void setTextureRepeat(textureT* texture, bool value);

#endif // texture_h_
