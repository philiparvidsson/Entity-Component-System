#version 430

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

void main() {
    mat4 mvp = Proj * View * Model;

    gl_Position = mvp * vec4(Position, 1.0);
}
