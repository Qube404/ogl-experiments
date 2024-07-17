#version 330 core

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
    mat3 TBN;
} fs_in;

out vec4 fragColor;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 viewPos;
uniform vec3 lightPos;

void main() {
    vec3 lightColor = texture(diffuseMap, fs_in.texCoords).rgb;

    vec3 normal = texture(normalMap, fs_in.texCoords).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(fs_in.TBN * normal); 

    float ambientStr = 0.1;
    vec3 ambient = vec3(ambientStr);

    vec3 lightDir = normalize(lightPos - fs_in.fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(diff);

    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = vec3(spec);

    fragColor = vec4((ambient + diffuse + specular) * lightColor, 1.0);
}
