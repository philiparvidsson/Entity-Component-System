#version 430

/*------------------------------------------------
 * UNIFORMS
 *----------------------------------------------*/

layout (binding=0) uniform sampler2D Tex;

/*------------------------------------------------
 * INPUTS
 *----------------------------------------------*/

in vec2 uv;

/*------------------------------------------------
 * OUTPUTS
 *----------------------------------------------*/

out vec4 frag_color;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

void main() {
    vec2 lol = uv;
    lol.x *= (0.9+0.1*cos(uv.y*6.0));
    lol.y *= (0.9+0.1*sin(uv.x*7.0));

    float f = cos((lol.y*uv.x) * 80.0) * 0.3 + 0.7;
    frag_color = vec4(texture(Tex, lol).rgb * vec3(f, 1.0-f, 1.0), 1.0);

    /*float r = 0.004;
    vec2 uv0 = vec2(uv.x - r, uv.y - r);
    vec2 uv1 = vec2(uv.x - r, uv.y    );
    vec2 uv2 = vec2(uv.x - r, uv.y + r);
    vec2 uv3 = vec2(uv.x    , uv.y - r);
    vec2 uv4 = vec2(uv.x    , uv.y    );
    vec2 uv5 = vec2(uv.x    , uv.y + r);
    vec2 uv6 = vec2(uv.x + r, uv.y - r);
    vec2 uv7 = vec2(uv.x + r, uv.y    );
    vec2 uv8 = vec2(uv.x + r, uv.y + r);

    frag_color = vec4((texture(Tex, uv0) +
    texture(Tex, uv1) +
    texture(Tex, uv2) +
    texture(Tex, uv3) +
    texture(Tex, uv4) +
    texture(Tex, uv5) +
    texture(Tex, uv6) +
    texture(Tex, uv7) +
    texture(Tex, uv8)).rgb / 9.0, 1.0);*/
}
