#version 330 core

uniform sampler2D diffuseTexture1; 

in vec2 TexCoords;

out vec4 fragColor;

void main() {
    vec4 texColor = texture(diffuseTexture1, TexCoords);

    fragColor = texColor;
}
