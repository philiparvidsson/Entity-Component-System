#version 430

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

layout(location = 0) out vec4 color;

void main() {
    vec3 light_pos = vec3(0.0, 1.0, 0.0);
    vec3 diff      = normalize(gl_FragCoord.xyz - light_pos);
    vec3 normal    = (Model * vec4(Normal, 1.0)).xyz;

    float dp = dot(normal, diff);
    if (dp < 0.0) dp = 0.0;

    float light_mult = dp;


    float f = clamp(light_mult, 0.3, 1.0);
    color = vec4(vec3(1.0, 1.0, 1.0) * f, 1.0);
}
