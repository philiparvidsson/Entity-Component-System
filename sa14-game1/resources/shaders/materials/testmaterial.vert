#version 430

struct vertexDataT {
    vec4 pos;
    vec4 prev_pos;
    vec3 normal;
    vec2 tex_coord;
};

uniform mat4 ProjViewModel;
uniform mat4 PrevProjViewModel;

uniform mat4 NormalMatrix;

layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec2 tex_coord;

out vertexDataT vert;

vec3 deform(in vec3 v) {
    float a = cos(sqrt(v.y*v.y+v.z*v.z)*300.0)*0.3+0.7;
    float b = sin(sqrt(v.x*v.x+v.z*v.z)*30.0)*0.3+0.7;
    float c = cos(sqrt(v.x*v.x+v.y*v.y)*100.0)*0.3+0.7;

    return (v*vec3(a, b, c));
}

void main() {
    vec3 pos = deform(vert_pos);

    vert.pos       =  ProjViewModel     * vec4(pos        , 1.0);
    vert.prev_pos  =  PrevProjViewModel * vec4(pos        , 1.0);
    vert.normal    = (NormalMatrix      * vec4(vert_normal, 1.0)).xyz;
    vert.tex_coord =  tex_coord;

    gl_Position = vert.pos;
}
