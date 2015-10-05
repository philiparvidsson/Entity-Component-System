#version 330 core
#extension GL_ARB_shading_language_420pack : enable

struct vertexDataT {
    vec4 pos;
    vec4 prev_pos;
    vec3 normal;
    vec2 tex_coord;
};

uniform mat4 ModelViewProj;
uniform mat4 PrevModelViewProj;

uniform mat4 NormalMatrix;

layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec2 tex_coord;

out vertexDataT vert;

void main() {
    vert.pos       =  ModelViewProj     * vec4(vert_pos   , 1.0);
    vert.prev_pos  =  PrevModelViewProj * vec4(vert_pos   , 1.0);
    vert.normal    = (NormalMatrix      * vec4(vert_normal, 1.0)).xyz;
    vert.tex_coord =  tex_coord;

    gl_Position = vert.pos;
}
