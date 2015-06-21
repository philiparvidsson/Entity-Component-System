/*------------------------------------------------------------------------------
 * File: text.frag
 * Created: June 19, 2015
 * Last changed: June 19, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Fragment shader for drawing text. It basically just reads from a texture
 *   and draws it to the target buffer.
 *----------------------------------------------------------------------------*/

#version 430

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

out vec4 frag_color;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void main() {
    frag_color = texture(Tex, vec2(uv.x, 1.0 - uv.y));
}
