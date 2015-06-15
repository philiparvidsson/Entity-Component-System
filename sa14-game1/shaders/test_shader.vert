#version 430


layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;

layout(location = 1) out float light_mult;

uniform float SomeVal;
uniform mat4 ViewMatrix;

mat4 lookAt(vec3 eye, vec3 target) {
    vec3 up = vec3(0, 1, 0);
    vec3 zaxis = normalize(target - eye);
    vec3 xaxis = normalize(cross(up, zaxis));
    vec3 yaxis = cross(zaxis, xaxis);

    return (mat4(
         xaxis.x,          yaxis.x,          zaxis.x,         0,
         xaxis.y,          yaxis.y,          zaxis.y,         0,
         xaxis.z,          yaxis.z,          zaxis.z,         0,
        -dot(xaxis, eye), -dot(yaxis, eye), -dot(zaxis, eye), 1
    ));
}

vec4 rotera(vec4 pos) {
    float a = SomeVal * 2.0;
    float b = SomeVal * 1.2*0.0+0.9;
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

    mat4 rot = rotX * rotZ;

    return rot * pos;
}

vec4 rotAndStuff(vec4 pos) {
    mat4 transl = transpose(mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, -2.0,
        0.0, 0.0, 0.0, 1.0
    ));

    return transl * rotera(pos);
}

void main() {
    vec3 l = normalize(-vec3(0, -0.5, -1));
    vec3 n = normalize(rotera(Normal).xyz);

    float dp = dot(n, l);
    if (dp < 0.0) dp = 0.0;

    light_mult = dp;

    gl_Position = ViewMatrix * rotAndStuff(Position);
}
