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

uniform vec3  ColorCoeff;
uniform float RefractionMult;

layout(binding = 0) uniform sampler2D ScreenTex;

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

float noise(in vec2 v) {
    return fract(sin(dot(v, vec2(12.9898, 78.233) * (3+1))) * 43758.5453);
}

void main() {
    vec3 n = normalize(vert.normal);
    vec3 v = normalize(vert.pos.xyz - vec3(0.0, 0.0, 1.0));

    vec3  ambient  = vec3(1.0, 1.0, 1.0);
    vec3  diffuse  = vec3(0.0, 0.0, 0.0);
    vec3  specular = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < NumLights; i++) {
        lightSourceT light = Lights[i];

        vec3 l = normalize(light.pos - vert.pos.xyz);
        vec3 r = reflect(l, n);

        float diffuse_factor  = max(0.0, dot(l, n));
        float specular_factor = pow(max(0.0, dot(r, v)), 20.0);

        ambient  += light.ambient;
        diffuse  -= light.diffuse  * diffuse_factor * (1.0-ColorCoeff);
        specular += light.specular * specular_factor;
    }

    vec2  texel_size    = 1.0/textureSize(ScreenTex, 0);
    vec2  d             = reflect(v,n).xy;
    vec2  uv            = gl_FragCoord.st            * texel_size;
    vec2  uv_reflect    = uv + d    * RefractionMult * texel_size;
    vec2  uv_refract    = uv - n.xy * RefractionMult * texel_size * 1.5;
    vec3  reflect_color = texture(ScreenTex, uv_reflect).rgb;
    vec3  refract_color = texture(ScreenTex, uv_refract).rgb;
    float a             = 1.0 - 0.4*length(d.xy);
    vec3  mix_color     = a*reflect_color + (1.0 - a)*refract_color;

    color = vec4(mix_color*(ambient+diffuse) + specular, a);
}
