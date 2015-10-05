#version 330 core
#extension GL_ARB_shading_language_420pack : enable

struct vertexDataT {
    vec4 pos;
    vec4 prev_pos;
    vec3 normal;
    vec2 tex_coord;
};

in vertexDataT vert;

out vec4 velocity;

void main() {
    vec2 pos      = (vert.pos.xy      / vert.pos.w     );
    vec2 prev_pos = (vert.prev_pos.xy / vert.prev_pos.w);

    velocity = vec4((pos - prev_pos).xy, 0.0, 1.0);
}
