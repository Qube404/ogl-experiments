#include <vector>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"

enum Shape {
    Cube,
    Square,
    Sphere,
    Cylinder,
    Plane,
    Cone,
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
};

class Texture {
public:
    unsigned int id;
    aiTextureType type;
    std::string path;

    Texture(unsigned int id, aiTextureType type, std::string path);
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void draw(Shader &shader);

private:
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};


class Model {
public:
    glm::vec3 position;
    float scale;

    Model();
    Model(std::vector<Mesh> meshes, glm::vec3 position = glm::vec3(0.f), float scale = 0.f);
    Model(std::string path, glm::vec3 position = glm::vec3(0.f), float scale = 0.f);

    void draw(Shader &shader);
private:
    std::vector<Mesh> meshes;
};
