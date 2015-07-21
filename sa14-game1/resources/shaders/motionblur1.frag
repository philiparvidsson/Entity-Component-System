#version 430

uniform int   MaxSamples     = 10;
uniform float VelocityFactor = 1.5;

layout(binding = 0) uniform sampler2D ColorTex;
layout(binding = 1) uniform sampler2D VelocityTex;

in vec2 uv;

out vec4 color;

void main() {
    vec2  texel_size  = 1.0 / textureSize(ColorTex, 0);
    vec2  velocity    = VelocityFactor * texture(VelocityTex, uv).rg;
    float speed       = length(velocity / texel_size);
    int   num_samples = clamp(int(speed), 1, MaxSamples);

    //velocity *= texel_size;

    color = vec4(0.0, 0.0, 0.0, 0.0);

    for (int i = 0; i < num_samples; i++) {
        float a = float(i) / float(num_samples);
        vec2 offset = velocity * (a - 0.5);
        color += texture(ColorTex, uv + offset);
    }

    color /= float(num_samples);
    color.a = 1.0;
}
