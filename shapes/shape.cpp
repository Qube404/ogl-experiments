#include "shape.h"

Cube::Cube(glm::vec3 pos, float size, glm::vec4 color) {
    float half = size / 2;

    // Bottom Left Closest Face
    vertices[0] = glm::vec3(pos.x - half, pos.y - half, pos.z - half);

    // Top Left Closest Face
    vertices[1] = glm::vec3(pos.x - half, pos.y + half, pos.z - half);

    // Top Right Closest Face
    vertices[2] = glm::vec3(pos.x + half, pos.y + half, pos.z - half);

    // Bottom Right Closest Face
    vertices[3] = glm::vec3(pos.x + half, pos.y - half, pos.z - half);

    // Bottom Left Closest Face
    vertices[4] = glm::vec3(pos.x - half, pos.y - half, pos.z + half);

    // Top Left Closest Face
    vertices[5] = glm::vec3(pos.x - half, pos.y + half, pos.z + half);

    // Top Right Closest Face
    vertices[6] = glm::vec3(pos.x + half, pos.y + half, pos.z + half);

    // Bottom Right Closest Face
    vertices[7] = glm::vec3(pos.x + half, pos.y - half, pos.z + half);
}
