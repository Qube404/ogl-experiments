#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
    mat3 TBN;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.texCoords = aTexCoords;
    vs_out.TBN = mat3(T, B, N);

    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
