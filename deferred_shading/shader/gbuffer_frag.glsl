#version 330 core

layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gNorm;
layout (location = 2) out vec4 gAlbedoSpec;

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

uniform sampler2D textureDiffuse1;
uniform sampler2D textureSpecular1;

void main() {
    gPos = fs_in.fragPos;
    gNorm = fs_in.normal;
    gAlbedoSpec.rgb = texture(textureDiffuse1, fs_in.texCoords).rgb;
    gAlbedoSpec.a = texture(textureSpecular1, fs_in.texCoords).r;
}
