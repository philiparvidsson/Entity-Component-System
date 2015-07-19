#version 430

struct fragDataT {
    vec3 pos;
    vec3 normal;
    vec2 tex_coord;
};

struct lightSourceT {
    vec3 pos;
    vec3 intensity;
};

struct materialT {
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float shininess;
};

uniform lightSourceT Lights[10];
uniform int NumLights;

uniform materialT Material;

layout(binding = 0) uniform sampler2D Texture;

in fragDataT frag;

out vec4 color;

void main() {
    vec3 n = normalize(frag.normal);
    vec3 v = normalize(frag.pos - vec3(0.0, 0.0, 1.0));

    vec3 illum = Material.ambient;

    for (int i = 0; i < NumLights; i++) {
        vec3 l = normalize(Lights[i].pos - frag.pos);
        vec3 r = reflect(l, n);

        illum += Material.diffuse*max(0.0, dot(l, n))*Lights[i].intensity
              +  Material.specular*pow(max(0.0, dot(r, v)), Material.shininess)*Lights[i].intensity;
    }

    color = vec4(illum, 1.0) * texture(Texture, frag.tex_coord);
}
