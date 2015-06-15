#version 430

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;

layout(location = 1) out float light_mult;

uniform float SomeVal;

uniform mat4  Proj;
uniform mat4  View;

mat4 Model;

void setModel() {
    float a = SomeVal * 2.0*0.0;
    float b = SomeVal * 1.2;
    float c = SomeVal;

    mat4 rotZ = transpose(mat4(
        cos(a), -sin(a), 0.0, 0.0,
        sin(a),  cos(a), 0.0, 0.0,
           0.0,     0.0, 1.0, 0.0,
           0.0,     0.0, 0.0, 1.0
    ));

    mat4 rotX = transpose(mat4(
        1.0,    0.0,     0.0, 0.0,
        0.0, cos(b), -sin(b), 0.0,
        0.0, sin(b),  cos(b), 0.0,
        0.0,    0.0,     0.0, 1.0
    ));

    mat4 rotY = transpose(mat4(
        cos(c),    0.0,     sin(c), 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sin(c), 0.0,  cos(c), 0.0,
        0.0,    0.0,     0.0, 1.0
    ));

    mat4 rot = rotY;

    mat4 transl = transpose(mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, -1.0,
        0.0, 0.0, 0.0, 1.0
    ));


    Model = transl * rotY;
}

void main() {
    setModel();
    float c = SomeVal;

        mat4 rotY = transpose(mat4(
        cos(c),    0.0,     sin(c), 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sin(c), 0.0,  cos(c), 0.0,
        0.0,    0.0,     0.0, 1.0
    ));


    vec3 l = normalize(-vec3(0.5, -1.5, -1.0));
    vec3 n = normalize((rotY * Normal).xyz);

    float dp = dot(n, l);
    if (dp < 0.0) dp = 0.0;

    light_mult = dp;


    mat4 mvp = Proj * View * Model;


    gl_Position = mvp * Position;
}
