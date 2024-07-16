#version 330 core

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
    vec4 fragPosLightSpace;
} fs_in;

out vec4 fragColor;

uniform sampler2D shadowMap;

uniform vec3 viewPos;
uniform vec3 lightPos;

float shadowCalc(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    bias = 0;

    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    if (projCoords.z > 1.0) {
        shadow = 0.0;
    }

    return shadow;
}

void main() {
    vec3 lightColor = vec3(1.0);
    vec3 normal = normalize(fs_in.normal);

    float ambientStr = 0.1;
    vec3 ambient = ambientStr * lightColor;

    vec3 lightDir = normalize(lightPos - fs_in.fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = spec * lightColor;

    float shadowValue = shadowCalc(fs_in.fragPosLightSpace, normal, lightDir);
    fragColor = vec4((ambient + (1.0 - shadowValue) * (diffuse + specular)) * lightColor, 1.0);
}
