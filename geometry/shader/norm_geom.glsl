#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

uniform mat4 proj;

in VS_OUT {
    vec3 norm;
} gs_in[];

const float mag = 0.4;

void generateLine(int idx) {
    gl_Position = proj * gl_in[idx].gl_Position;
    EmitVertex();

    gl_Position = proj * (gl_in[idx].gl_Position + vec4(gs_in[idx].norm, 0.0) * mag);
    EmitVertex();

    EndPrimitive();
}

void main() {
    generateLine(0);
    generateLine(1);
    generateLine(2);
}
