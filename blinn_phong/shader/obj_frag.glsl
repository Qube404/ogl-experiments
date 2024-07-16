#version 330 core

in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;

void main() {
    vec3 lightColor = vec3(1.0);

    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfDir = normalize(lightDir + viewDir);

    float ambientStr = 0.1;
    vec3 ambient = ambientStr * lightColor;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float spec = pow(max(dot(normal, halfDir), 0.0), 32);
    vec3 specular = spec * lightColor;

    fragColor = vec4(min(ambient + diffuse + specular, 1.0), 1.0);
}
