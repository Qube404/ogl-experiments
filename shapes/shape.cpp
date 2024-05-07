#include "shape.h"

Cube::Cube(glm::vec3 pos, float size, glm::vec4 color): VBO(0), EBO(0), VAO(0) {
    float half = size / 2;

    // Problem is not the vertices
    vertices[0] = glm::vec3(pos.x - half, pos.y - half, pos.z - half);
    vertices[1] = glm::vec3(pos.x - half, pos.y + half, pos.z - half);
    vertices[2] = glm::vec3(pos.x + half, pos.y + half, pos.z - half);
    vertices[3] = glm::vec3(pos.x + half, pos.y - half, pos.z - half);

    // Furthest Bottom Left Clockwise Face Verts
    vertices[4] = glm::vec3(pos.x - half, pos.y - half, pos.z + half);
    vertices[5] = glm::vec3(pos.x - half, pos.y + half, pos.z + half);
    vertices[6] = glm::vec3(pos.x + half, pos.y + half, pos.z + half);
    vertices[7] = glm::vec3(pos.x + half, pos.y - half, pos.z + half);

    // Face - Closest
    indices[0][0] = 0;
    indices[0][1] = 1;
    indices[0][2] = 2;

    indices[1][0] = 0;
    indices[1][1] = 2;
    indices[1][2] = 3;

    // Face - Furthest
    indices[2][0] = 4;
    indices[2][1] = 5;
    indices[2][2] = 6;

    indices[3][0] = 4;
    indices[3][1] = 6;
    indices[3][2] = 7;

    // Face - Left
    indices[4][0] = 4;
    indices[4][1] = 5;
    indices[4][2] = 1;

    indices[5][0] = 4;
    indices[5][1] = 1;
    indices[5][2] = 0;

    // Face - Right
    indices[6][0] = 7;
    indices[6][1] = 6;
    indices[6][2] = 2;

    indices[7][0] = 7;
    indices[7][1] = 2;
    indices[7][2] = 3;

    // Face - Top
    indices[8][0] = 1;
    indices[8][1] = 5;
    indices[8][2] = 6;

    indices[9][0] = 1;
    indices[9][1] = 6;
    indices[9][2] = 2;

    // Face - Bottom
    indices[10][0] = 0;
    indices[10][1] = 4;
    indices[10][2] = 7;

    indices[11][0] = 0;
    indices[11][1] = 7;
    indices[11][2] = 3;

    // Setup Mesh
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, (long int) (VERTEX_NO * sizeof(glm::vec3)), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long int) sizeof(indices), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*) 0);

    glBindVertexArray(0);
}


void Cube::draw(Shader &shader) {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (int) (INDICE_NO * INDICE_COMP), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Cube::clean() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
