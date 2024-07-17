#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D hdrBuffer;

uniform float exposure;

vec3 reinhardToneMapping(float gamma) {
    vec3 hdrColor = texture(hdrBuffer, texCoords).rgb;

    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    mapped = pow(mapped, vec3(1.0 / gamma));

    return mapped;
}

vec3 exposureToneMapping(float gamma, float exposure) {
    vec3 hdrColor = texture(hdrBuffer, texCoords).rgb;

    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));

    return mapped;
}

void main() {
    const float gamma = 2.2;
    vec3 color = exposureToneMapping(gamma, exposure);

    fragColor = vec4(color, 1.0);
}
