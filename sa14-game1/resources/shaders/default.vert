#version 430

struct fragDataT {
    vec3 pos;
    vec3 normal;
    vec2 tex_coord;
};

uniform mat4 ProjViewModel;
uniform mat4 NormalTransform;

layout(location = 0) in vec3 VertPos;
layout(location = 1) in vec3 VertNormal;
layout(location = 2) in vec2 TexCoord;

out fragDataT frag;

void main() {
    gl_Position = ProjViewModel * vec4(VertPos, 1.0);

    frag.pos       = (ProjViewModel * vec4(VertPos, 1.0)).xyz;
    frag.normal    = (NormalTransform * vec4(VertNormal, 0.0)).xyz;
    frag.tex_coord = TexCoord;
}
