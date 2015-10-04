/*------------------------------------------------------------------------------
 * File: expsure.frag
 * Created: September 10, 2015
 * Last changed: September 10, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Exposure shader for adjusting color brightness.
 *----------------------------------------------------------------------------*/

// @To-do: Investigate why this shader makes the colors look more realistic.
//         Clues might be found here:
//             http://freespace.virgin.net/hugo.elias/graphics/x_posure.htm

#version 330
#extension GL_ARB_shading_language_420pack: require

/*------------------------------------------------
 * UNIFORMS
 *----------------------------------------------*/

layout(binding = 0) uniform sampler2D Tex;

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
    color = vec4(sqrt(texture(Tex, uv).rgb), 1.0);
}
