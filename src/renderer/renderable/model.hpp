#ifndef MODEL_H_
#define MODEL_H_

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <iostream>

#include "shader.hpp"
#include "shader_engine.hpp"
#include "primitive.hpp"
#include "renderable.hpp"
#include "texture.hpp"


class Mesh : public Renderable {
public:
    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices,
         std::vector<Texture>& textures);
};

class Model {
public:
    Model(std::string const path);
    Model(Primitive& primitive);
    void draw();
    void setShaderEngine(ShaderEngine engine);
    std::vector<Renderable> getMeshes() {
        return m_meshes;
    }
private:
    std::vector<Renderable> m_meshes;
    std::string m_directory;
    std::vector<Texture> m_texturesLoaded;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
        aiTextureType assimpTextureType, TextureType lambTextureType);
};

#endif
