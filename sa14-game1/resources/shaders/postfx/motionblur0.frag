#version 430

struct vertexT {
    vec4 pos;
    vec4 prev_pos;
};

in vertexT vert;

out vec4 velocity;

void main() {
    vec2 pos      = (vert.pos.xy      / vert.pos.w     );
    vec2 prev_pos = (vert.prev_pos.xy / vert.prev_pos.w);

    velocity = vec4((pos - prev_pos).xy, 0.0, 1.0);
}
