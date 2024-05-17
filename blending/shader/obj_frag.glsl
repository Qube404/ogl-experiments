#version 330 core

uniform sampler2D shapeTexture; 

in vec2 TexCoords;

out vec4 fragColor;

void main() {
    vec4 texColor = texture(shapeTexture, TexCoords);

    if (texColor.a <= 0.1) {
        discard;
    }

    fragColor = texColor;
}
