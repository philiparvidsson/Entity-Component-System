#version 430

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 UV;

layout(location = 0) out vec4 color;

layout(binding = 0) uniform sampler2D snabel;

void main() {
    vec3 light_pos = vec3(-0.05, 1.0, 1.85);

    vec3 n = (Model * vec4(Normal, 1.0)).xyz;
    vec3 d = (View * vec4(light_pos, 1.0)).xyz - (View * Model * vec4(Vertex, 1.0)).xyz;

    n = normalize(n);
    d = normalize(d);

    vec2 q = (Proj * View * Model * vec4(Normal, 1.0)).xy;


    float l=clamp(dot(n, d), 0.0, 1.0);
    float fq = l+0.1;
    //color = vec4(vec3(1.0, 1.0, 1.0) * f, 1.0);

    vec2 uv = UV;
    vec2 lol = uv;
    lol.x *= (0.9+0.1*cos(uv.y*6.0));
    lol.y *= (0.9+0.1*sin(uv.x*7.0));

    float f = 1.0;//cos((lol.y*uv.x) * 80.0) * 0.3 + 0.7;
    color = vec4(vec3(f, 1.0, 1.0) * fq, 1.0);
}
