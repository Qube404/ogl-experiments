#version 330 core

in vec2 TexCoords;

out vec4 fragColor;

float near = 0.1;
float far = 100.0;

void main() {
    float depth = gl_FragCoord.z;
    float ndc = depth * 2.0 - 1.0;
    float linearDepth = (2.0 * near * far) / (far + near - ndc * (far - near)) / far;

    fragColor = vec4(vec3(linearDepth), 1.0);
}
