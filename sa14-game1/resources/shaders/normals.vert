#version 430

uniform mat4 ModelTransform;
uniform mat4 NormalTransform;
uniform mat4 View;
uniform mat4 Proj;

layout(location = 0) in vec3 VertPos;
layout(location = 1) in vec3 VertNormal;
layout(location = 2) in vec2 TexCoord;

out vertDataT {
    vec3 pos;
    vec3 normal;
} vert;

void main() {
    vert.pos    = (ModelTransform  * vec4(VertPos   , 1.0)).xyz;
    vert.normal = (NormalTransform * vec4(VertNormal, 0.0)).xyz;
}
