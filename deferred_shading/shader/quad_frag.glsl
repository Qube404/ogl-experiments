#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D positions;
uniform sampler2D normals;
uniform sampler2D albedoSpecs;

uniform vec3 viewPos;

struct Light {
    vec3 pos;
    vec3 color;
    float radius;
};
const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];

void main() {
    vec3 fragPos = texture(positions, texCoords).rgb;
    vec3 normal = texture(normals, texCoords).rgb;
    vec3 albedo = texture(albedoSpecs, texCoords).rgb;
    float roughness = texture(albedoSpecs, texCoords).a;

    vec3 lighting = albedo * 0.1;
    vec3 viewDir = normalize(viewPos - fragPos);

    for (int i = 0; i != NR_LIGHTS; i++) {
        float distance = length(lights[i].pos - fragPos);
        if (distance < lights[i].radius) {
            vec3 lightDir = normalize(lights[i].pos - fragPos);
            vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * lights[i].color;

            lighting += diffuse;
        }
    }

    fragColor = vec4(lighting, 1.0);
}
