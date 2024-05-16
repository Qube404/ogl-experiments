#version 330 core

uniform sampler2D shapeTexture; 

in vec2 TexCoords;

out vec4 fragColor;

void main() {
    fragColor = texture(shapeTexture, TexCoords);
}
