#version 430

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec3 Normal;

layout(location = 0) out vec3 vertex;
layout(location = 1) out vec3 normal;

void main() {
    vertex = Vertex;
    normal = Normal;

    mat4 mvp = Proj * View * Model;

    gl_Position = mvp * vec4(Vertex, 1.0);
}
