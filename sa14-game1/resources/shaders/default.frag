#version 430

layout(location = 0) out vec4 color;

layout(binding = 0) uniform sampler2D Texture;

in vertDataT {
    vec3 normal;
    vec2 tex_coord;
} vert;

void main() {
    vec3 n = normalize(vert.normal);
    vec3 d = -normalize(vec3(-0.5, -0.5, -1.0));

    float l=clamp(dot(n, d), 0.0, 1.0);
    float fq = l+0.1;

    float f = 1.0;;
    color = vec4(vec3(f, 1.0, 1.0) * fq, 1.0) * texture(Texture, vert.tex_coord);
}
