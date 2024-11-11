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
    std::vector<unsigned int> indices;
    for (int face = 0; face < 6; ++face) {
        unsigned int baseIndex = face * 4;

        indices.push_back(baseIndex + 2);
        indices.push_back(baseIndex);
        indices.push_back(baseIndex + 3);

        indices.push_back(baseIndex);
        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 3);
    }
    return indices;
}

std::vector<float> Cube::computeVertices() {
    // Half size for easier positioning
    float halfSize = m_scale / 2.0f;

    std::vector<float> vertices;
    // Loop over 6 faces (4 vertices per face)
    for (int face = 0; face < 6; ++face) {
        glm::vec3 normal;
        glm::vec2 texCoords[4];

        // Determine the normal and texture coordinates for each face
        switch (face) {
            case 0:  // Front face
                normal = glm::vec3(0.0f, 0.0f, 1.0f);
                texCoords[0] = glm::vec2(0.0f, 0.0f); // Bottom-left
                texCoords[1] = glm::vec2(1.0f, 0.0f); // Bottom-right
                texCoords[2] = glm::vec2(0.0f, 1.0f); // Top-left
                texCoords[3] = glm::vec2(1.0f, 1.0f); // Top-right
                break;
            case 1:  // Back face
                normal = glm::vec3(0.0f, 0.0f, -1.0f);
                texCoords[0] = glm::vec2(1.0f, 0.0f);
                texCoords[1] = glm::vec2(0.0f, 0.0f);
                texCoords[2] = glm::vec2(1.0f, 1.0f);
                texCoords[3] = glm::vec2(0.0f, 1.0f);
                break;
            case 2:  // Right face
                normal = glm::vec3(1.0f, 0.0f, 0.0f);
                texCoords[0] = glm::vec2(0.0f, 0.0f);
                texCoords[1] = glm::vec2(1.0f, 0.0f);
                texCoords[2] = glm::vec2(0.0f, 1.0f);
                texCoords[3] = glm::vec2(1.0f, 1.0f);
                break;
            case 3:  // Left face
                normal = glm::vec3(-1.0f, 0.0f, 0.0f);
                texCoords[0] = glm::vec2(1.0f, 0.0f);
                texCoords[1] = glm::vec2(0.0f, 0.0f);
                texCoords[2] = glm::vec2(1.0f, 1.0f);
                texCoords[3] = glm::vec2(0.0f, 1.0f);
                break;
            case 4:  // Top face
                normal = glm::vec3(0.0f, 1.0f, 0.0f);
                texCoords[0] = glm::vec2(0.0f, 0.0f);
                texCoords[1] = glm::vec2(1.0f, 0.0f);
                texCoords[2] = glm::vec2(0.0f, 1.0f);
                texCoords[3] = glm::vec2(1.0f, 1.0f);
                break;
            case 5:  // Bottom face
                normal = glm::vec3(0.0f, -1.0f, 0.0f);
                texCoords[0] = glm::vec2(0.0f, 1.0f);
                texCoords[1] = glm::vec2(1.0f, 1.0f);
                texCoords[2] = glm::vec2(0.0f, 0.0f);
                texCoords[3] = glm::vec2(1.0f, 0.0f);
                break;
        }

        // Create 4 vertices per face and calculate positions
        for (int i = 0; i < 4; ++i) {
            float x = (i % 2 == 0) ? -halfSize : halfSize;  // Alternates between -halfSize and halfSize
            float z = (i / 2 == 0) ? -halfSize : halfSize; // Alternates between -halfSize and halfSize

            // Determine position based on the face
            glm::vec3 position;
            switch (face) {
                case 0: position = glm::vec3(x, z, -halfSize); break;  // Front face
                case 1: position = glm::vec3(x, z, halfSize); break;   // Back face
                case 2: position = glm::vec3(halfSize, z, x); break;   // Right face
                case 3: position = glm::vec3(-halfSize, z, x); break;  // Left face
                case 4: position = glm::vec3(x, halfSize, z); break;   // Top face
                case 5: position = glm::vec3(x, -halfSize, z); break;  // Bottom face
            }

            // Add the vertex with position, normal, and texture coordinates
            // Vertex vertex;
            // vertex.position = position;
            // vertex.normal = normal;
            // vertex.texCoords = texCoords[i];
            vertices.push_back(position.x);
            vertices.push_back(position.y);
            vertices.push_back(position.z);

            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
        
            vertices.push_back(texCoords[i].x);
            vertices.push_back(texCoords[i].y);
        }
    }

    for (int i = 0; i < vertices.size(); i += 8) {
        std::cout << vertices[i] << ", " << vertices[i + 1] << ", " << vertices[i + 2] << std::endl;
    }

    return vertices;
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
