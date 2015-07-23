#ifndef rendertarget_h_
#define rendertarget_h_

#include "base/common.h"
#include "graphics/texture.h"

typedef struct renderTargetT renderTargetT;

renderTargetT* createFloatRenderTarget(int width, int height);
renderTargetT* createMultisampledRenderTarget(int width, int height,
                                              int num_samples);
renderTargetT* createRenderTarget(int width, int height);
void freeRenderTarget(renderTargetT* render_target);
renderTargetT* useRenderTarget(renderTargetT* render_target);
void presentRenderTarget(renderTargetT* render_target);
textureT* getRenderTargetColorTexture(renderTargetT* rt);
textureT* getRenderTargetDepthTexture(renderTargetT* rt);

#endif // rendertarget_h_
