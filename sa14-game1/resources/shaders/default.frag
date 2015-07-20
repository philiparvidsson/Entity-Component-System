#version 430

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

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

/*------------------------------------------------
 * UNIFORMS
 *----------------------------------------------*/

uniform lightSourceT Lights[10];
uniform int NumLights;

uniform materialT Material;

layout(binding = 0) uniform sampler2D Texture;

/*------------------------------------------------
 * INPUTS
 *----------------------------------------------*/

in fragDataT frag;

/*------------------------------------------------
 * OUTPUTS
 *----------------------------------------------*/

out vec4 color;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void main() {
    vec3 n = normalize(frag.normal);
    vec3 v = normalize(frag.pos - vec3(0.0, 0.0, 1.0));

    vec3 ambient  = Material.ambient;
    vec3 diffuse  = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < NumLights; i++) {
        lightSourceT light = Lights[i];

        vec3 l = normalize(light.pos - frag.pos);
        vec3 r = reflect(l, n);

        float diffuse_factor  = max(0.0, dot(l, n));
        float specular_factor = pow(max(0.0, dot(r, v)), Material.shininess);

        ambient  += light.ambient;
        diffuse  += Material.diffuse  * light.diffuse  * diffuse_factor;
        specular += Material.specular * light.specular * specular_factor;
    }

    vec4 tex_color = texture(Texture, frag.tex_coord);
    color = vec4((ambient+diffuse) * tex_color.rgb + specular, tex_color.a);
}
