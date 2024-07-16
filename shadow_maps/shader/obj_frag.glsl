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

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    for (int x = -1; x != 1; x++) {
        for (int y = -1; y != 1; y++) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

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
