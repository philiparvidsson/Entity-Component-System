#version 430

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 UV;

layout(location = 0) out vec3 vertex;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec2 uv;

void main() {
    vertex = Vertex;
    normal = Normal;
    uv = UV;

    mat4 pvm = Proj * View * Model;

    vec3 Vertex2 = Vertex;
    gl_Position = pvm * vec4(Vertex2, 1.0);
}
