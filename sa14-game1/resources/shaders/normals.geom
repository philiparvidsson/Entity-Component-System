#version 430

uniform mat4 View;
uniform mat4 Proj;
uniform float NormalLength = 0.01;

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in vertDataT {
    vec3 pos;
    vec3 normal;
} verts[];

void main() {
    mat4 pv = Proj * View;

    for (int i = 0; i < gl_in.length(); i++) {
        vec4 normal = vec4(verts[i].normal, 0.0);

        gl_Position = pv * vec4(verts[i].pos, 1.0);
        EmitVertex();

        gl_Position = pv * vec4(verts[i].pos+verts[i].normal*NormalLength, 1.0);
        EmitVertex();

        EndPrimitive();
    }
}
