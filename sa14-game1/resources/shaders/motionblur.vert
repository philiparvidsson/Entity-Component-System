#version 430

struct vertexT {
    vec4 pos;
    vec4 prev_pos;
};

uniform mat4 ProjViewModel;
uniform mat4 PrevProjViewModel;

layout(location = 0) in vec3 vert_pos;

out vertexT vert;

void main() {
    vert.pos      = ProjViewModel     * vec4(vert_pos, 1.0);
    vert.prev_pos = PrevProjViewModel * vec4(vert_pos, 1.0);

    gl_Position = vert.pos;
}
