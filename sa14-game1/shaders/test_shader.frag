#version 150

uniform float some_val;

out vec4 final_color;

void main() {
    final_color = vec4(some_val, 1.0, 1.0-some_val, 0.0);
}
