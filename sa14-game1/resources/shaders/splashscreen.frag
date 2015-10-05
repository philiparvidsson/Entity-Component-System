/*------------------------------------------------------------------------------
 * File: splashscreen.vert
 * Created: July 14, 2015
 * Last changed: July 14, 2015
 *
 * Author(s): Philip Arvidsson (contact@philiparvidsson.com)
 *
 * Description:
 *   Provides a splash screen shader that fades between white and a specified
 *   splash screen texture.
 *----------------------------------------------------------------------------*/

#version 330 core
#extension GL_ARB_shading_language_420pack : enable

/*------------------------------------------------
 * UNIFORMS
 *----------------------------------------------*/

layout (binding=0) uniform sampler2D SplashTex;
uniform float Fade;

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
    float fade   = clamp(Fade, 0.0, 1.0);
    vec4  color0 = vec4(1.0, 1.0, 1.0, 1.0);
    vec4  color1 = texture(SplashTex, uv);

    frag_color = color0*fade + color1*(1.0-fade);
}
