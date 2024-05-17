#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "model.h"
#include "stb_image.h"

std::vector<Texture> texturesLoaded;

Texture::Texture(unsigned int id, aiTextureType type, std::string path):
    id(id),
    type(type),
    path(path)
{}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures):
    vertices(vertices), indices(indices), textures(textures),
    VAO(0), VBO(0), EBO(0)
{
    setupMesh();
}

void Mesh::draw(Shader &shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i != textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        aiTextureType textype = textures[i].type;

        std::string number;
        std::string name;

        if (textype == aiTextureType::aiTextureType_DIFFUSE) {
            name = "textureDiffuse";
            number = std::to_string(diffuseNr++);
        } else if (textype == aiTextureType::aiTextureType_SPECULAR) {
            name = "textureSpecular";
            number = std::to_string(specularNr++);
        }

        shader.setInt((name + number).c_str(), (int) i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (int) indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, (long int) (vertices.size() * sizeof(Vertex)), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long int) (indices.size() * sizeof(unsigned int)), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

Model::Model(): meshes(), position(0.f), scale(1.f) {}

Model::Model(std::vector<Mesh> meshes, glm::vec3 position, float scale, float rotation, glm::vec3 angle): 
    meshes(meshes), position(position), scale(scale), rotation(rotation), angle(angle) 
{}

unsigned int textureFromFile(std::string path, std::string dir) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    std::string fullPath(path + '/' + dir);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = GL_FALSE;
        switch (nrChannels) {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, (GLint) format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    } else {
        std::cout << "Failed to load texture at path: " << path << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}

std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string dir) {
    std::vector<Texture> textures;
    
    for (unsigned int i = 0; i != mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j != texturesLoaded.size(); j++) {
            if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(texturesLoaded[j]);
                skip = true;

                break;
            }
        }

        if (!skip) {
            Texture texture(
                textureFromFile(str.C_Str(), dir),
                type,
                str.C_Str()
            );

            textures.push_back(texture);
            texturesLoaded.push_back(texture);
        }
    }

    return textures;
}

Mesh processMesh(aiMesh *mesh, const aiScene *scene, std::string dir) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i != mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        vertex.normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );

        if (mesh->mTextureCoords[0]) {
            vertex.texCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i != mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j != face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps = loadMaterialTextures(
        material, 
        aiTextureType_DIFFUSE,
        dir
    );
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTextures(
        material,
        aiTextureType_SPECULAR,
        dir
    );
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    return Mesh(vertices, indices, textures);
}

void processNode(std::vector<Mesh> &meshes, aiNode *node, const aiScene *scene, std::string dir) {
    for (unsigned int i = 0; i != node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, dir));
    }

    for (unsigned int i = 0; i != node->mNumChildren; i++) {
        processNode(meshes, node->mChildren[i], scene, dir);
    }
}

Model::Model(std::string path, glm::vec3 position, float scale, float rotation, glm::vec3 angle): 
    meshes(), position(position), scale(scale), rotation(rotation), angle(angle)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    std::string dir = path.substr(0, path.find_last_of('/'));
    processNode(meshes, scene->mRootNode, scene, dir);
}


void Model::draw(Shader &shader) {
    for (unsigned int i = 0; i != meshes.size(); i++) {
        meshes[i].draw(shader);
    }
}

#endif
