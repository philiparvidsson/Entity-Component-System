#version 430

layout(location = 0) out vec4 color;
layout(location = 1) in float light_mult;

void main() {
    float lm = clamp(light_mult, 0.3, 1.0);
    color = vec4(vec3(1.0, 1.0, 1.0) * lm, 1.0);
}
