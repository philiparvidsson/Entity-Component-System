#version 430

uniform float NormalLength = 0.01;

uniform mat4 ProjViewModel;
uniform mat4 PrevProjViewModel;

layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec3 vert_normal;

out vertexDataT {
    vec4 pos0;
    vec4 pos1;
} vert;

void main() {
    vert.pos0 =  ProjViewModel * vec4(vert_pos, 1.0);
    vert.pos1 =  ProjViewModel * vec4(vert_pos + vert_normal*NormalLength, 1.0);

    gl_Position = vert.pos0;
}
