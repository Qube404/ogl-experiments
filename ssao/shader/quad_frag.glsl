#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D positions;
uniform sampler2D normals;
uniform sampler2D albedo;
uniform sampler2D ssaoInput;

struct Light {
    vec3 pos;
    vec3 color;

    float linear;
    float quadratic;
};
uniform Light light;

void main() {
    vec3 fragPos = texture(positions, texCoords).rgb;
    vec3 normal = texture(normals, texCoords).rgb;
    vec3 albedo = texture(albedo, texCoords).rgb;
    float ambientOcclusion = texture(ssaoInput, texCoords).r;

    vec3 ambient = vec3(0.3 * albedo * ambientOcclusion);
    vec3 lighting = ambient;
    vec3 viewDir = normalize(-fragPos);

    vec3 lightDir = normalize(light.pos - fragPos);
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * light.color;

    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfDir), 0.0), 16.0);
    vec3 specular = light.color * spec;

    float dist = length(light.pos - fragPos);
    float attenuation = 1.0 / (1.0 + light.linear * dist + light.quadratic * dist * dist);
    diffuse *= attenuation;
    specular *= attenuation;
    lighting += diffuse + specular;

    fragColor = vec4(lighting, 1.0);
    fragColor = vec4(albedo, 1.0);
}
