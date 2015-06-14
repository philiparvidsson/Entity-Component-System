#version 430

uniform float some_val;

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

layout(location = 1) out float light_mult;

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

void main() {
    float a = some_val * 2.0;
    float b = some_val * 1.2;

    mat4 rotZ = mat4(
        cos(a), -sin(a), 0.0, 0.0,
        sin(a),  cos(a), 0.0, 0.0,
           0.0,     0.0, 1.0, 0.0,
           0.0,     0.0, 0.0, 1.0
    );

    mat4 rotX = mat4(
        1.0,    0.0,     0.0, 0.0,
        0.0, cos(b), -sin(b), 0.0,
        0.0, sin(b),  cos(b), 0.0,
        0.0,    0.0,     0.0, 1.0
    );

    mat4 rot = transpose(rotX) * rotZ;
    vec3 target = vec3(0, 0, 0);
    vec3 eye = vec3(0, 0, -1);

    vec3 light = normalize(vec3(-1, 1, -1));
    vec4 lol = transpose(lookAt(eye, target)) * rot * vec4(normal, 1.0);
    float dp = dot(lol.xyz, light);

    light_mult = dp;

    gl_Position = transpose(lookAt(eye, target)) * rot * position;
}
