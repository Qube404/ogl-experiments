#version 330 core

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

out vec4 fragColor;

uniform samplerCube shadowMap;

uniform vec3 viewPos;
uniform vec3 lightPos;

uniform float farPlane;

float shadowCalc(vec3 fragPos, vec3 normal, vec3 lightDir) {
    vec3 fragToLight = fragPos - lightPos;
    float closestDepth = texture(shadowMap, fragToLight).r;
    closestDepth *= farPlane;

    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.05;
    float samples = 4.0;
    float offset = 0.1;

    for (float x = -offset; x != offset; x += offset / (samples * 0.5)) {
        for (float y = -offset; y != offset; y += offset / (samples * 0.5)) {
            for (float z = -offset; z != offset; z += offset / (samples * 0.5)) {
                float closestDepth = texture(shadowMap, fragToLight + vec3(x, y, z)).r;
                closestDepth *= farPlane;

                if (currentDepth - bias > closestDepth) {
                    shadow += 1.0;
                }
            }
        }
    }
    shadow /= (samples * samples * samples);

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

    float shadowValue = shadowCalc(fs_in.fragPos, normal, lightDir);
    fragColor = vec4((ambient + (1.0 - shadowValue) * (diffuse + specular)) * lightColor, 1.0);
}
