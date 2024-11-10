#include <primitive.hpp>
#include <glad/glad.h>
#include <vector>
#include <iostream>


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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Primitive::draw() {
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
        4, 0, 3,
        3, 7, 4,
        // Right face
        1, 5, 6,
        6, 2, 1,
        // Top face
        3, 2, 6,
        6, 7, 3,
        // Bottom face
        4, 5, 1,
        1, 0, 4
    };
}

std::vector<float> Cube::computeVertices() {
    return {
        -0.5f * m_scale, -0.5f * m_scale,  0.5f * m_scale,  // Front face
         0.5f * m_scale, -0.5f * m_scale,  0.5f * m_scale,
         0.5f * m_scale,  0.5f * m_scale,  0.5f * m_scale,
        -0.5f * m_scale,  0.5f * m_scale,  0.5f * m_scale,
        -0.5f * m_scale, -0.5f * m_scale, -0.5f * m_scale,  // Back face
         0.5f * m_scale, -0.5f * m_scale, -0.5f * m_scale,
         0.5f * m_scale,  0.5f * m_scale, -0.5f * m_scale,
        -0.5f * m_scale,  0.5f * m_scale, -0.5f * m_scale
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
