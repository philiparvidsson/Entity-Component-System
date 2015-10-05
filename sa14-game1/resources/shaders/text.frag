/*------------------------------------------------------------------------------
 * File: text.frag
 * Created: June 19, 2015
 * Last changed: June 19, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Fragment shader for drawing text. It basically just reads from a texture
 *   and draws it to the target buffer.
 *----------------------------------------------------------------------------*/

#version 330 core
#extension GL_ARB_shading_language_420pack : enable

/*------------------------------------------------
 * UNIFORMS
 *----------------------------------------------*/

layout (binding=0) uniform sampler2D Tex;

/*------------------------------------------------
 * INPUTS
 *----------------------------------------------*/

in vec2 uv;

/*------------------------------------------------
 * OUTPUTS
 *----------------------------------------------*/

out vec4 color;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void main() {
    color = texture(Tex, uv);
}
