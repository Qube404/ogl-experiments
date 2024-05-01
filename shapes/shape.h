#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "shader.h"

class Cube {
public:
    Cube(glm::vec3 pos, float size, glm::vec4 color);
    void draw(Shader &shader);

private:
    glm::vec3 vertices[8];
    unsigned int indices[36];
    glm::vec4 color;
};

#endif
