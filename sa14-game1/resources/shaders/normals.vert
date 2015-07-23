#version 430


uniform mat4 ProjViewModel;
uniform mat4 PrevProjViewModel;

uniform mat4 NormalMatrix;

layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec2 tex_coord;

out vertexDataT {
    vec4 pos;
    vec4 prev_pos;
    vec3 normal;
    vec2 tex_coord;
} vert;

void main() {
    vert.pos       =  ProjViewModel     * vec4(vert_pos   , 1.0);
    vert.prev_pos  =  PrevProjViewModel * vec4(vert_pos   , 1.0);
    vert.normal    = (NormalMatrix      * vec4(vert_normal, 1.0)).xyz;
    vert.tex_coord =  tex_coord;

    gl_Position = vert.pos;
}
