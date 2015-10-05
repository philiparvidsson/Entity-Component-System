#version 330 core
#extension GL_ARB_shading_language_420pack : enable

/*------------------------------------------------
 * UNIFORMS
 *----------------------------------------------*/

layout (binding=0) uniform sampler2D Tex;
layout (binding=0) uniform vec2 Velo = vec2(0.0, 0.0);

/*------------------------------------------------
 * INPUTS
 *----------------------------------------------*/

in vec2 uv;

/*------------------------------------------------
 * OUTPUTS
 *----------------------------------------------*/

out vec4 color;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void main() {
    /*vec2 lol = uv;
    lol.x *= (0.9+0.1*cos(uv.y*6.0));
    lol.y *= (0.9+0.1*sin(uv.x*7.0));

    float f = cos((lol.y*uv.x) * 80.0) * 0.3 + 0.7;
    frag_color = vec4(texture(Tex, lol).rgb * vec3(f, 1.0-f, 1.0), 1.0);*/

    /*float r = 0.002;
    vec2 uv0 = vec2(uv.x - r, uv.y - r);
    vec2 uv1 = vec2(uv.x - r, uv.y    );
    vec2 uv2 = vec2(uv.x - r, uv.y + r);
    vec2 uv3 = vec2(uv.x    , uv.y - r);
    vec2 uv4 = vec2(uv.x    , uv.y    );
    vec2 uv5 = vec2(uv.x    , uv.y + r);
    vec2 uv6 = vec2(uv.x + r, uv.y - r);
    vec2 uv7 = vec2(uv.x + r, uv.y    );
    vec2 uv8 = vec2(uv.x + r, uv.y + r);

    vec4 cb = texture(Tex, uv);
    vec4 ca = vec4((texture(Tex, uv0) +
    texture(Tex, uv1) +
    texture(Tex, uv2) +
    texture(Tex, uv3) +
    texture(Tex, uv4) +
    texture(Tex, uv5) +
    texture(Tex, uv6) +
    texture(Tex, uv7) +
    texture(Tex, uv8)).rgb / 9.0, 1.0);*/

    color = vec4(0.0, 0.0, 0.0, 1.0);

    float pixel_w = 1.0f / 1280.0;
    float pixel_h = 1.0 / 720.0;

    vec2 v = Velo;
    int num_samples = int(max(v.x, v.y)+0.5);
    v *= (1.0f / num_samples);

    vec2 tex_coord = uv;
    for (int i = 0; i < num_samples; i++)  {
        color += vec4(texture(Tex, tex_coord).rgb, 0.0);
        tex_coord += v * vec2(pixel_w, pixel_h);
    }

    color /= num_samples;
    color.a = 1.0;
}
