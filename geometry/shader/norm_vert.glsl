#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;

out VS_OUT {
    vec3 norm;
} vs_out;

void main() {
    gl_Position = view * model * vec4(aPos, 1.0);
    mat3 normMatrix = mat3(transpose(inverse(view * model)));
    vs_out.norm = normalize(vec3(vec4(normMatrix * aNormal, 0.0)));
}
