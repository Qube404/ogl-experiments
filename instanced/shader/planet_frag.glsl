#version 330 core

in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D textureDiffuse1;

void main() {
    fragColor = texture(textureDiffuse1, TexCoord);
}
