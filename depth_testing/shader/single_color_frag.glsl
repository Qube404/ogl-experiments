#version 330 core

uniform sampler2D shapeTexture; 

in vec2 TexCoords;

out vec4 fragColor;

float near = 0.1;
float far = 100.0;

void main() {
    fragColor = vec4(1.0);
}
