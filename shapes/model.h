#include <vector>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
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
    Model();
    Model(std::vector<Mesh> meshes);
    Model(std::string path);

    void draw(Shader &shader);

private:
    std::vector<Mesh> meshes;
};

/*
class Model {
public:
    Model();
    Model(std::string path);
    Model(std::vector<Mesh> meshes);

    void draw(Shader &shader);

private:
    std::vector<Mesh> meshes;
    std::string dir;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type);
};
*/
