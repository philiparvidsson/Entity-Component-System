#version 430

/*------------------------------------------------
 * UNIFORMS
 *----------------------------------------------*/

uniform int   MaxSamples     = 32;
uniform float VelocityFactor = 2.4;

layout(binding = 0) uniform sampler2D ColorTex;
layout(binding = 1) uniform sampler2D VelocityTex;

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

vec2 sampledVelocity(in vec2 uv) {
    vec2 r = 8.0 / textureSize(ColorTex, 0);
    vec2 v = vec2(0.0, 0.0);

    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++)
            v += texture(VelocityTex, uv + vec2(i, j) * r).rg;
    }

    v *= 1.0/25.0;

    return (v);
}

void main() {
    vec2  texel_size  = 1.0 / textureSize(ColorTex, 0);
    vec2  velocity    = VelocityFactor * sampledVelocity(uv);
    float speed       = length(velocity / texel_size);
    int   num_samples = clamp(int(speed), 1, MaxSamples);

    color = vec4(0.0, 0.0, 0.0, 0.0);

    for (int i = 0; i < num_samples; i++) {
        float a = float(i) / float(num_samples);
        vec2 offset = velocity * (a - 0.5);
        color += texture(ColorTex, uv + offset);
    }

    color /= float(num_samples);
    color.a = 1.0;
}
