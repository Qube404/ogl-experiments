#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D scene;
uniform sampler2D bloomBlur;

uniform float exposure;

vec3 exposureToneMapping(vec3 color, float gamma, float exposure) {
    vec3 mapped = vec3(1.0) - exp(-color * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));

    return mapped;
}

void main() {
    const float gamma = 2.2;

    vec3 hdrColor = texture(scene, texCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, texCoords).rgb;
    hdrColor += bloomColor;

    vec3 color = exposureToneMapping(hdrColor, gamma, exposure);

    fragColor = vec4(color, 1.0);
}
