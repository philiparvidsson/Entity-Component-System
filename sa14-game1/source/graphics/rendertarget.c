#include "rendertarget.h"

#include "base/common.h"
#include "graphics/graphics.h"
#include "graphics/texture.h"

#include <GL/glew.h>

struct renderTargetT {
    GLuint fbo;
    GLuint z_buffer;

    int width;
    int height;

    GLuint tex_id;
};

static renderTargetT* active_render_target = NULL;

renderTargetT* createFloatRenderTarget(int width, int height) {
    renderTargetT* render_target = malloc(sizeof(renderTargetT));

    render_target->width = width;
    render_target->height = height;

    glGenFramebuffers(1, &render_target->fbo);

    textureT*      old_texture       = useTexture(NULL, 0);
    renderTargetT* old_render_target = useRenderTarget(render_target);

    glGenTextures(1, &render_target->tex_id);
    glBindTexture(GL_TEXTURE_2D, render_target->tex_id);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glGenRenderbuffers       (1, &render_target->z_buffer);
    glBindRenderbuffer       (GL_RENDERBUFFER, render_target->z_buffer);
    glRenderbufferStorage    (GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_target->z_buffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_target->tex_id, 0);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    useRenderTarget(old_render_target);
    useTexture     (old_texture, 0);

    return (render_target);
}

renderTargetT* createMultisampledRenderTarget(int width, int height, int num_samples) {

    renderTargetT* render_target = malloc(sizeof(renderTargetT));

    render_target->width = width;
    render_target->height = height;

    glGenFramebuffers(1, &render_target->fbo);

    textureT*      old_texture       = useTexture(NULL, 0);
    renderTargetT* old_render_target = useRenderTarget(render_target);

    glGenTextures(1, &render_target->tex_id);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, render_target->tex_id);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, num_samples, GL_RGBA8, width, height, GL_TRUE);

    glGenRenderbuffers              (1, &render_target->z_buffer);
    glBindRenderbuffer              (GL_RENDERBUFFER, render_target->z_buffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, num_samples, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer       (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_target->z_buffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, render_target->tex_id, 0);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    useRenderTarget(old_render_target);
    useTexture     (old_texture, 0);

    return (render_target);
}

renderTargetT* createRenderTarget(int width, int height) {
    renderTargetT* render_target = malloc(sizeof(renderTargetT));

    render_target->width = width;
    render_target->height = height;

    glGenFramebuffers(1, &render_target->fbo);

    textureT*      old_texture       = useTexture(NULL, 0);
    renderTargetT* old_render_target = useRenderTarget(render_target);

    glGenTextures(1, &render_target->tex_id);
    glBindTexture(GL_TEXTURE_2D, render_target->tex_id);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glGenRenderbuffers       (1, &render_target->z_buffer);
    glBindRenderbuffer       (GL_RENDERBUFFER, render_target->z_buffer);
    glRenderbufferStorage    (GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_target->z_buffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_target->tex_id, 0);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    useRenderTarget(old_render_target);
    useTexture     (old_texture, 0);

    return (render_target);
}

void freeRenderTarget(renderTargetT* render_target) {
    if (render_target == active_render_target)
        useRenderTarget(NULL);

    glDeleteTextures(1, &render_target->tex_id);
    glDeleteRenderbuffers(1, &render_target->z_buffer);
    glDeleteFramebuffers(1, &render_target->fbo);
    free(render_target);
}

renderTargetT* useRenderTarget(renderTargetT* render_target) {
    renderTargetT* old_render_target = active_render_target;

    if (render_target == old_render_target)
        return (old_render_target);

    active_render_target = render_target;

    if (!render_target) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, screenWidth(), screenHeight());
        return (old_render_target);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, render_target->fbo);
    glViewport(0, 0, render_target->width, render_target->height);

    return (old_render_target);
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
