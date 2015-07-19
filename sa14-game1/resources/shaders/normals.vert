#version 430

uniform mat4 ModelTransform;
uniform mat4 NormalTransform;
uniform mat4 View;
uniform mat4 Proj;

layout(location = 0) in vec3 VertPos;
layout(location = 1) in vec3 VertNormal;
layout(location = 2) in vec2 TexCoord;

out vertDataT {
    vec3 normal;
} vert;

void main() {
    mat4 pv  = Proj * View;
    mat4 pvm = pv * ModelTransform;
    mat4 pvn = pv * NormalTransform;

    vert.normal = (pvn * vec4(VertNormal, 0.0)).xyz;

    gl_Position = pvm * vec4(VertPos, 1.0);
}
