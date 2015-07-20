#version 430

struct fragDataT {
    vec3 pos;
    vec3 normal;
    vec2 tex_coord;
};

struct lightSourceT {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
        lightSourceT light = Lights[i];

        vec3 l = normalize(light.pos - frag.pos);
        vec3 r = reflect(l, n);

        float diffuse_factor  = max(0.0, dot(l, n));
        float specular_factor = pow(max(0.0, dot(r, v)), Material.shininess);

        vec3 ambient  = Material.ambient  * light.ambient;
        vec3 diffuse  = Material.diffuse  * light.diffuse  * diffuse_factor;
        vec3 specular = Material.specular * light.specular * specular_factor;

        illum += ambient + diffuse + specular;
    }

    color = vec4(illum, 1.0) * texture(Texture, frag.tex_coord);
}
