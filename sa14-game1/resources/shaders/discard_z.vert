/*------------------------------------------------------------------------------
 * File: discard_z.vert
 * Created: June 21, 2015
 * Last changed: June 21, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   A shader that discards the z-coordinate and does nothing else.
 *----------------------------------------------------------------------------*/

#version 430

/*------------------------------------------------
 * INPUTS
 *----------------------------------------------*/

layout (location=0) in vec3 vert_pos;
layout (location=2) in vec2 tex_coord;

/*------------------------------------------------
 * OUTPUTS
 *----------------------------------------------*/

out vec2 uv;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void main() {
    uv = tex_coord;
    gl_Position = vec4(vert_pos.xy, 0.0, 1.0);
}
