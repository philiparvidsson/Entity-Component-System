#version 430

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

struct vertexDataT {
    vec4 pos;
    vec4 prev_pos;
    vec3 normal;
    vec2 tex_coord;
};

struct lightSourceT {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

/*------------------------------------------------
 * UNIFORMS
 *----------------------------------------------*/

uniform lightSourceT Lights[10];
uniform int NumLights;

uniform vec3  AmbientCoeff;
uniform vec3  DiffuseCoeff;
uniform vec3  SpecularCoeff;
uniform float Shininess;

layout(binding = 0) uniform sampler2D AmbientTex;
layout(binding = 1) uniform sampler2D DiffuseTex;
layout(binding = 2) uniform sampler2D SpecularTex;

/*------------------------------------------------
 * INPUTS
 *----------------------------------------------*/

in vertexDataT vert;

/*------------------------------------------------
 * OUTPUTS
 *----------------------------------------------*/

out vec4 color;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void main() {
    vec3 n = normalize(vert.normal);
    vec3 v = normalize(vert.pos.xyz - vec3(0.0, 0.0, 1.0));

    vec3 ambient  = AmbientCoeff;
    vec3 diffuse  = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < NumLights; i++) {
        lightSourceT light = Lights[i];

        vec3 l = normalize(light.pos - vert.pos.xyz);
        vec3 r = reflect(l, n);

        float diffuse_factor  = max(0.0, dot(l, n));
        float specular_factor = pow(max(0.0, dot(r, v)), Shininess);

        ambient  += light.ambient;
        diffuse  += DiffuseCoeff  * light.diffuse  * diffuse_factor;
        specular += SpecularCoeff * light.specular * specular_factor;
    }

    ambient  *= texture(AmbientTex , vert.tex_coord).rgb;
    diffuse  *= texture(DiffuseTex , vert.tex_coord).rgb;
    specular *= texture(SpecularTex, vert.tex_coord).rgb;

    color = vec4((ambient+diffuse) + specular, 1.0);
}
