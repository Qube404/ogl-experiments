#version 330 core

layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gNorm;
layout (location = 2) out vec3 gAlbedo;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

void main() {
    gPos = fs_in.fragPos;
    gNorm = fs_in.normal;
    gAlbedo = vec3(0.95);
}
