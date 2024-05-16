#version 330 core

in vec2 TexCoords;

out vec4 fragColor;

uniform vec4 singleColor;

void main() {
    fragColor = singleColor;
}
