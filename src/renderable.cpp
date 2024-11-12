#include <renderable.hpp>
#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"
#include <texture.hpp>

void Renderable::setup() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    if (m_VAO == 0 || m_VBO == 0 || m_EBO == 0) {
        std::cerr << "Error: Failed to generate buffers!" << std::endl;
        return;
    }

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
                    &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
                    &m_indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Renderable::draw() {
    m_engine.use();
    if (!m_textures.empty()) {
        unsigned int diffuseNumber = 1, specularNumber = 1;
        for (int i = 0; i < m_textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            TextureType type = m_textures[i].type;
            switch (type) {
                case TextureType::DIFFUSE:
                    number = std::to_string(diffuseNumber++);
                    break;
                case TextureType::SPECULAR:
                    number = std::to_string(specularNumber++);
                    break;
            };

            std::string typeStr = toString(type);
            std::string textureUniformName = "material." + typeStr + number;
            m_engine.setInt(textureUniformName.c_str(), i); // c_str() need to be called directly, otherwise pointer will be lose.
            glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
        }
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Renderable::setTexture(const char* path, TextureType type) {
    std::string filename(path);

    Texture texture;
    texture.type = type;
    texture.path = std::string(path);
    glGenTextures(1, &texture.id);
    if (texture.id == 0) {
        std::cerr << "Error: Failed to generate texture ID!" << std::endl;
        return;
    }

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);

        m_textures.push_back(texture);
    } else {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
}

std::ostream& operator<<(std::ostream& os, const Renderable& renderable) {
    os << "Renderable Object:\n";
    os << "Vertices:\n";
    for (const auto& vertex : renderable.m_vertices) {
        os << "  Position: (" << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << "), ";
        os << "Normal: (" << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << "), ";
        os << "Texture Coordinates: (" << vertex.textureCoordinates.x << ", " << vertex.textureCoordinates.y << ")\n";
    }

    os << "Indices:\n";
    for (const auto& index : renderable.m_indices) {
        os << "  " << index << " ";
    }
    os << "\n";

    os << "Textures:\n";
    for (const auto& texture : renderable.m_textures) {
        os << "  ID: " << texture.id << ", Type: " << texture.type << ", Path: " << texture.path << "\n";
    }
    
    return os;
}
