#ifndef MODEL_H_
#define MODEL_H_

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.hpp"
#include <primitive.hpp>


class Mesh {
public:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
        std::vector<Texture> textures) {
        m_vertices = vertices;
        m_indices = indices;
        m_textures = textures;

        setupMesh();
    };
    void draw(ShaderEngine &shader);
private:
    unsigned int m_VAO, m_VBO, m_EBO;
    void setupMesh();
};

class Model {
    public:
        Model(std::string const path);
        void draw(ShaderEngine& shader);
    private:
        std::vector<Mesh> m_meshes;
        std::string m_directory;
        std::vector<Texture> m_texturesLoaded;

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
};

#endif
