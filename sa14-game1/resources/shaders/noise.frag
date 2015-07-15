#version 430

/*------------------------------------------------
 * UNIFORMS
 *----------------------------------------------*/

uniform float Intensity;
uniform uint  Seed;

layout(binding=0) uniform sampler2D Tex;

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

float noise(in vec2 v) {
    return fract(sin(dot(v, vec2(12.9898, 78.233) * (Seed+1))) * 43758.5453);
}

void main() {
    vec3 color0 = vec3(1.0, 1.0, 1.0) * noise(uv);
    vec3 color1 = texture(Tex, uv).rgb;

    frag_color = vec4(color0*Intensity + color1*(1.0-Intensity), 1.0);
}
