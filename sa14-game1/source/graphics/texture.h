#ifndef texture_h_
#define texture_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "base/common.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

typedef struct textureT textureT;

/*------------------------------------------------
 * FUNCTIOMS
 *----------------------------------------------*/

textureT* createMultisampledTexture(void);
textureT* createTexture(void);
textureT* loadTextureFromFile(const void* file_name);
textureT* loadTextureFromMemory(const void* data);
void loadTextureFromScreen(textureT* tex);
textureT* useTexture(textureT* texture, int index);
void freeTexture(textureT* texture);
bool getTextureRepeat(const textureT* texture);
void setTextureRepeat(textureT* texture, bool value);
textureT* whiteTexture(void);

#endif // texture_h_
