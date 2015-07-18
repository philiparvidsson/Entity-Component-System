#version 430

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in vertDataT {
    vec3 normal;
    vec2 tex_coord;
} verts[];

void main() {
    for (int i = 0; i < gl_in.length(); i++) {
        vec4 normal = vec4(verts[i].normal, 0.0);

        gl_Position = gl_in[i].gl_Position;
        EmitVertex();

        gl_Position += normal * 0.02;
        EmitVertex();

        EndPrimitive();
    }
}
