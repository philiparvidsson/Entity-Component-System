/*------------------------------------------------------------------------------
 * File: text.vert
 * Created: June 19, 2015
 * Last changed: June 19, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Vertex shader for drawing text. It zeroes the z-coordinate as if the input
 *   coordinates were actually two-dimensional.
 *----------------------------------------------------------------------------*/

#version 330 core
#extension GL_ARB_shading_language_420pack : enable

/*------------------------------------------------
 * UNIFORMS
 *----------------------------------------------*/

 uniform vec2 ScreenSize;
 uniform vec4 TextRect;

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

    float x = -1.0 + (vert_pos.x + 1.0) * (TextRect.z / ScreenSize.x)
            + (2.0 * TextRect.x / ScreenSize.x);
    float y =  1.0 - (vert_pos.y + 1.0) * (TextRect.w / ScreenSize.y)
            - (2.0 * TextRect.y / ScreenSize.y);

    gl_Position = vec4(x, y, 0.0, 1.0);
}
