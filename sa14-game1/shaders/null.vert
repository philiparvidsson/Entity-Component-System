#version 150

uniform float some_val;

in vec3 vert;

void main() {
    gl_Position = vec4(vert, some_val);
}
