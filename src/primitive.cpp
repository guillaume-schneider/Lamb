#include <primitive.hpp>
#include <glad/glad.h>
#include <vector>
#include <string>
#include <iostream>
#include <shader.hpp>
#include "stb_image.h"


#ifndef PI
#define PI 3.14159265358979323846
#endif


void Primitive::setupDrawing() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    if (m_VAO == 0 || m_VBO == 0 || m_EBO == 0) {
        std::cerr << "Error: Failed to generate buffers!" << std::endl;
        return;
    }

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float),
                    &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
                    &m_indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Primitive::draw(ShaderEngine& engine) {
    if (m_textures.size() > 0) {
        for (int i = 0; i < m_textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            engine.setInt("texture" + std::to_string(m_textures[i].id), i);
            glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
        }
    }

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::vector<unsigned int> Cube::computeIndices() {
    return {
        // Front face
        0, 1, 2, // B, C, D
        2, 3, 0, // D, A, B

        // Back face
        4, 5, 6, 
        6, 7, 4, 

        // Left face
        8, 9, 10,
        10, 11, 8, 

        // Right face
        12, 13, 14,
        14, 15, 12,

        // Top face
        16, 17, 18,
        18, 19, 16,

        // Bottom face
        20, 21, 22,
        22, 23, 20
    };
}

std::vector<float> Cube::computeVertices() {
    return {
        -0.5f * m_scale, -0.5f * m_scale,  0.5f * m_scale, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Bottom-left
         0.5f * m_scale, -0.5f * m_scale,  0.5f * m_scale, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // Bottom-right
         0.5f * m_scale,  0.5f * m_scale,  0.5f * m_scale, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // Top-right
        -0.5f * m_scale,  0.5f * m_scale,  0.5f * m_scale, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // Top-left

        // Back face (z = -0.5)
        -0.5f * m_scale, -0.5f * m_scale, -0.5f * m_scale, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // Bottom-left
         0.5f * m_scale, -0.5f * m_scale, -0.5f * m_scale, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // Bottom-right
         0.5f * m_scale,  0.5f * m_scale, -0.5f * m_scale, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // Top-right
        -0.5f * m_scale,  0.5f * m_scale, -0.5f * m_scale, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // Top-left

        // Left face (x = -0.5)
        -0.5f * m_scale, -0.5f * m_scale, -0.5f * m_scale, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Bottom-left
        -0.5f * m_scale, -0.5f * m_scale,  0.5f * m_scale, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom-right
        -0.5f * m_scale,  0.5f * m_scale,  0.5f * m_scale, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // Top-right
        -0.5f * m_scale,  0.5f * m_scale, -0.5f * m_scale, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top-left

        // Right face (x = 0.5)
         0.5f * m_scale, -0.5f * m_scale, -0.5f * m_scale, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Bottom-left
         0.5f * m_scale, -0.5f * m_scale,  0.5f * m_scale, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Bottom-right
         0.5f * m_scale,  0.5f * m_scale,  0.5f * m_scale, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // Top-right
         0.5f * m_scale,  0.5f * m_scale, -0.5f * m_scale, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Top-left

        // Top face (y = 0.5)
        -0.5f * m_scale,  0.5f * m_scale, -0.5f * m_scale, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // Bottom-left
         0.5f * m_scale,  0.5f * m_scale, -0.5f * m_scale, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // Bottom-right
         0.5f * m_scale,  0.5f * m_scale,  0.5f * m_scale, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // Top-right
        -0.5f * m_scale,  0.5f * m_scale,  0.5f * m_scale, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // Top-left

        // Bottom face (y = -0.5)
        -0.5f * m_scale, -0.5f * m_scale, -0.5f * m_scale, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // Bottom-left
         0.5f * m_scale, -0.5f * m_scale, -0.5f * m_scale, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // Bottom-right
         0.5f * m_scale, -0.5f * m_scale,  0.5f * m_scale, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // Top-right
        -0.5f * m_scale, -0.5f * m_scale,  0.5f * m_scale, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f   // Top-left
    };
}

std::vector<float> Sphere::computeVertices() {
    float stackAngle, sectorAngle;
    float x, y, z;

    std::vector<float> vertices;
    for (int i = 0; i <= m_stackCount; i ++){
        stackAngle = PI / 2 - (PI * float(i) / float(m_stackCount));

        for (int j = 0; j <= m_sectorCount; j ++) {
            sectorAngle = 2 * PI * (float(j) / float(m_sectorCount));

            x = m_radius * cosf(stackAngle) * cosf(sectorAngle);
            y = m_radius * sinf(stackAngle);
            z = m_radius * cosf(stackAngle) * sinf(sectorAngle);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    return vertices;
}

std::vector<unsigned int> Sphere::computeIndices() {
    std::vector<unsigned int> indices;
    unsigned int k1, k2;
    for (int i = 0; i < m_stackCount; i++) {
        k1 = i * (m_sectorCount + 1);
        k2 = k1 + m_sectorCount + 1;
        for (int j = 0; j < m_sectorCount; j++, k1++, k2++) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (m_stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    return indices;
}

void Primitive::setTexture(const char* path) {
    std::string filename(path);
    
    Texture texture;
    texture.path = std::string(path);
    glGenTextures(1, &texture.id);
    if (texture.id == 0) {
        std::cerr << "Error: Failed to generate texture ID!" << std::endl;
        return;
    }

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RGB;  // Default format
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set texture wrapping and filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        m_textures.push_back(texture);
    } else {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
}
