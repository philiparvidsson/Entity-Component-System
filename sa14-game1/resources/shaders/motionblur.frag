#version 430

struct vertexT {
    vec4 pos;
    vec4 prev_pos;
};

in vertexT vert;

out vec2 velocity;

void main() {
    vec2 pos      = (vert.pos.xy      / vert.pos.w     ) * 0.5;
    vec2 prev_pos = (vert.prev_pos.xy / vert.prev_pos.w) * 0.5;

    velocity = 100.0f * (pos - prev_pos) + vec2(0.5, 0.5);
}
