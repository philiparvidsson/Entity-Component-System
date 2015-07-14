#ifndef texture_h_
#define texture_h_

#include "base/common.h"

typedef struct textureT textureT;

void freeTexture(textureT* texture);
textureT* useTexture(textureT* texture, int index);
textureT* createTextureFromScreen(void);
bool getTextureRepeat(textureT* texture);
void setTextureRepeat(textureT* texture, bool value);

#endif // texture_h_
