#version 430

uniform float NormalLength = 0.1;

uniform mat4 ModelViewProj;
uniform mat4 PrevModelViewProj;

layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec3 vert_normal;

out vertexDataT {
    vec4 pos0;
    vec4 pos1;
} vert;

void main() {
    vert.pos0 =  ModelViewProj * vec4(vert_pos, 1.0);
    vert.pos1 =  ModelViewProj * vec4(vert_pos + vert_normal*NormalLength, 1.0);

    gl_Position = vert.pos0;
}
