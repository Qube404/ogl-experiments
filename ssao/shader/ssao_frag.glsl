#version 330 core

in vec2 texCoords;

out float fragColor;

uniform sampler2D gPos;
uniform sampler2D gNorm;
uniform sampler2D texNoise;

uniform vec3 samples[64];
uniform mat4 proj;

const vec2 noiseScale = vec2(1920.0/4.0, 1080/4.0);

void main() {
    vec3 fragPos = texture(gPos, texCoords).xyz;
    vec3 norm = texture(gNorm, texCoords).rgb;
    vec3 randomVec = texture(texNoise, texCoords * noiseScale).xyz;

    vec3 tangent = normalize(randomVec - norm * dot(randomVec, norm));
    vec3 bitangent = cross(norm, tangent);
    mat3 TBN = mat3(tangent, bitangent, norm);

    float kernelSize = 64;
    float radius = 0.5;
    float bias = 0.025;

    float occlusion = 0.0;
    for (int i = 0; i != kernelSize; i++) {
        vec3 samplePos = TBN * samples[i];
        samplePos = fragPos + samplePos * radius;

        vec4 offset = vec4(samplePos, 1.0);
        offset = proj * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = texture(gPos, offset.xy).z;

        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / kernelSize);
    fragColor = occlusion;
}
