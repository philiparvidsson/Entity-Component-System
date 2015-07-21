#include "rendertarget.h"

#include "base/common.h"
#include "graphics/graphics.h"
#include "graphics/texture.h"

#include <GL/glew.h>

struct renderTargetT {
    GLuint fbo;

    int width;
    int height;

    textureT* color_tex;
    textureT* depth_tex;
};

static renderTargetT* active_render_target = NULL;

renderTargetT* createMultisampledRenderTarget(int width, int height, int num_samples) {
    renderTargetT* rt = malloc(sizeof(renderTargetT));

    rt->width  = width;
    rt->height = height;
    
    glGenFramebuffers(1, &rt->fbo);

    textureT*      old_tex = useTexture(NULL, 0);
    renderTargetT* old_rt = useRenderTarget(rt);

    rt->color_tex = createMultisampledTexture();
    rt->depth_tex = createMultisampledTexture();

    useTexture(rt->color_tex, 0);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, num_samples, GL_RGBA8,
                            width, height, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D_MULTISAMPLE, *(GLuint*)rt->color_tex,
                           0);

    useTexture(rt->depth_tex, 0);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, num_samples,
                            GL_DEPTH_COMPONENT32, width, height, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D_MULTISAMPLE, *(GLuint*)rt->depth_tex,
                           0);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    useRenderTarget(old_rt);
    useTexture     (old_tex, 0);

    return (rt);
}

renderTargetT* createRenderTarget(int width, int height) {
    renderTargetT* rt = malloc(sizeof(renderTargetT));

    rt->width  = width;
    rt->height = height;
    
    glGenFramebuffers(1, &rt->fbo);

    textureT*      old_tex = useTexture(NULL, 0);
    renderTargetT* old_rt  = useRenderTarget(rt);

    rt->color_tex = createTexture();
    rt->depth_tex = createTexture();

    useTexture(rt->color_tex, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
                 GL_FLOAT, NULL);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                         *(GLuint*)rt->color_tex, 0);

    useTexture(rt->depth_tex, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                         *(GLuint*)rt->depth_tex, 0);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    useRenderTarget(old_rt);
    useTexture     (old_tex, 0);

    return (rt);
}

void freeRenderTarget(renderTargetT* rt) {
    if (rt == active_render_target)
        useRenderTarget(NULL);

    freeTexture(rt->color_tex);
    freeTexture(rt->depth_tex);

    glDeleteFramebuffers(1, &rt->fbo);
    free(rt);
}

renderTargetT* useRenderTarget(renderTargetT* rt) {
    renderTargetT* old_rt = active_render_target;

    if (rt == old_rt)
        return (old_rt);

    active_render_target = rt;

    if (!rt) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, screenWidth(), screenHeight());
        return (old_rt);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, rt->fbo);
    glViewport(0, 0, rt->width, rt->height);

    return (old_rt);
}

void presentRenderTarget(renderTargetT* render_target) {
    renderTargetT* old_render_target = useRenderTarget(NULL);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, render_target->fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, render_target->width, render_target->height, 0, 0, screenWidth(), screenHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

    if (!old_render_target) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, screenWidth(), screenHeight());
    }
    else {
        useRenderTarget(old_render_target);
    }
}

textureT* getRenderTargetColorTexture(renderTargetT* rt) {
    return (rt->color_tex);
}

textureT* getRenderTargetDepthTexture(renderTargetT* rt) {
    return (rt->depth_tex);
}