#version 430

/*------------------------------------------------
 * UNIFORMS
 *----------------------------------------------*/

uniform float Intensity = 0.16;
uniform uint  Seed;

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

float noise(in vec2 v) {
    return fract(sin(dot(v, vec2(12.9898, 78.233) * (Seed+1))) * 43758.5453);
}

void main() {
    vec2  r = 1.0 / textureSize(Tex, 0);
    float a = 0.0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++)
            a += noise(uv + vec2(i, j)*r);
    }

    a /= 18.0;
    a += noise(uv)*0.5;

    vec3  color0 = vec3(a, a, a);
    vec3  color1 = texture(Tex, uv).rgb;
    float x      = 1.0 - pow((color1.r+color1.g+color1.b)/3.4, 2.0);

    x = clamp(x*Intensity, 0.0, 1.0);
    color = vec4(color0*x + color1*(1.0-x), 1.0);
}
