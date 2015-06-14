#version 430

layout(location = 0) out vec4 color;
layout(location = 1) in float LightMult;

void main() {
    float f = clamp(LightMult, 0.3, 1.0);
    color = vec4(vec3(1.0, 1.0, 1.0) * f, 1.0);
}
