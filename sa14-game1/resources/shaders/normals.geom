#version 430

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in vertexDataT {
    vec4 pos0;
    vec4 pos1;
} verts[];

void main() {
    for (int i = 0; i < gl_in.length(); i++) {
        gl_Position = verts[i].pos0;
        EmitVertex();

        gl_Position = verts[i].pos1;
        EmitVertex();

        EndPrimitive();
    }
}
