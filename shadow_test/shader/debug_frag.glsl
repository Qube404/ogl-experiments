#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D depthMap;

void main() {
    float depthValue = texture(depthMap, texCoords).r;
    fragColor = vec4(vec3(depthValue), 1.0);
}
