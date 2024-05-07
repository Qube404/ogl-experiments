#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "shader.h"

const unsigned int VERTEX_NO = 8;

const unsigned int INDICE_NO = 12;
const unsigned int INDICE_COMP = 3;

class Cube {
public:
    Cube(glm::vec3 pos, float size, glm::vec4 color);
    void draw(Shader &shader);
    void clean();

private:
    glm::vec3 vertices[VERTEX_NO];
    unsigned int indices[INDICE_NO][INDICE_COMP];
    glm::vec4 color;

    unsigned int VBO, EBO, VAO;
};

#endif
