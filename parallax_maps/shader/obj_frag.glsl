#version 330 core

in VS_OUT {
    vec3 fragPos;

    vec3 normal;
    vec2 texCoords;

    vec3 tangentFragPos;
    vec3 tangentViewPos;
    vec3 tangentLightPos;
} fs_in;

out vec4 fragColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D heightMap;

uniform float heightScale;

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir) {
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));
    
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;

    vec2 P = viewDir.xy * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    vec2 currentTexCoords = texCoords;
    float currentHeightMapValue = texture(heightMap, currentTexCoords).r;

    while (currentLayerDepth < currentHeightMapValue) {
        currentTexCoords -= deltaTexCoords;
        currentHeightMapValue = texture(heightMap, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth = currentHeightMapValue - currentLayerDepth;
    float beforeDepth = texture(heightMap, prevTexCoords).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main() {
    vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
    vec2 texCoords = parallaxMapping(fs_in.texCoords, viewDir);
    if (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0) {
        discard;
    }

    vec3 lightColor = texture(diffuseMap, texCoords).rgb;

    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    float ambientStr = 0.1;
    vec3 ambient = vec3(ambientStr);

    vec3 lightDir = normalize(fs_in.tangentLightPos - fs_in.tangentFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(diff);

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = vec3(spec);

    fragColor = vec4((ambient + diffuse + specular) * lightColor, 1.0);
}
