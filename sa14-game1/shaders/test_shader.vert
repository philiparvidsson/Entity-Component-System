#version 150

uniform float some_val;

in vec3 vert;

void main() {
    float a = some_val * 5.0;
    mat4 rot = mat4(
        cos(a), -sin(a), 0.0, 0.0,
        sin(a),  cos(a), 0.0, 0.0,
           0.0,     0.0, 1.0, 0.0,
           0.0,     0.0, 0.0, 1.0
    );

    gl_Position = vec4(vert, some_val) * rot;
}
