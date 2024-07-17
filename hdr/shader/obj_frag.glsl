#version 330 core

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

out vec4 fragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;

void main() {
    float ambientStr = 0.1;
    vec3 ambient = vec3(ambientStr);

    vec3 lightDir = normalize(lightPos - fs_in.fragPos);
    float diff = max(dot(fs_in.normal, lightDir), 0.0);
    vec3 diffuse = vec3(diff);

    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    vec3 reflectDir = reflect(-lightDir, fs_in.normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = vec3(spec);

    fragColor = vec4((ambient + diffuse + specular) * vec3(1.0), 1.0);
}
