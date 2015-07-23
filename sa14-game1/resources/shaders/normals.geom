#version 430

uniform mat4 View;
uniform mat4 Proj;
uniform float NormalLength = 0.01;

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in vertexDataT {
    vec4 pos;
    vec4 prev_pos;
    vec3 normal;
    vec2 tex_coord;
} verts[];

void main() {
    for (int i = 0; i < gl_in.length(); i++) {
        gl_Position = verts[i].pos;
        EmitVertex();

        gl_Position = verts[i].pos+vec4(verts[i].normal*NormalLength, 0.0);
        EmitVertex();

        EndPrimitive();
    }
}
