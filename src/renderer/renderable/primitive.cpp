#include <primitive.hpp>
#include <vector>


#ifndef PI
#define PI 3.14159265358979323846
#endif

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

std::vector<Vertex> Cube::computeVertices() {
    // Half size for easier positioning
    float halfSize = m_scale / 2.0f;

    std::vector<Vertex> vertices;
    // Loop over 6 faces (4 vertices per face)
    for (int face = 0; face < 6; ++face) {
        glm::vec3 normal;
        glm::vec2 texCoords[4];

        // Determine the normal and texture coordinates for each face
        switch (face) {
            case 0:  // Front face
                normal = glm::vec3(0.0f, 0.0f, -1.0f);
                texCoords[0] = glm::vec2(0.0f, 0.0f); // Bottom-left
                texCoords[1] = glm::vec2(1.0f, 0.0f); // Bottom-right
                texCoords[2] = glm::vec2(0.0f, 1.0f); // Top-left
                texCoords[3] = glm::vec2(1.0f, 1.0f); // Top-right
                break;
            case 1:  // Back face
                normal = glm::vec3(0.0f, 0.0f, 1.0f);
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

        for (int i = 0; i < 4; ++i) {
            float x = (i % 2 == 0) ? -halfSize : halfSize;
            float z = (i / 2 == 0) ? -halfSize : halfSize;

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

            Vertex vertex;
            vertex.position = position;
            vertex.normal = normal;
            vertex.textureCoordinates = texCoords[i];
            vertices.push_back(vertex);
        }
    }

    return vertices;
}

std::vector<Vertex> Sphere::computeVertices() {
    float stackAngle, sectorAngle;
    float x, y, z;

    std::vector<Vertex> vertices;
    for (int i = 0; i <= m_stackCount; i ++){
        stackAngle = PI / 2 - (PI * float(i) / float(m_stackCount));

        for (int j = 0; j <= m_sectorCount; j ++) {
            sectorAngle = 2 * PI * (float(j) / float(m_sectorCount));

            x = m_radius * cosf(stackAngle) * cosf(sectorAngle);
            y = m_radius * sinf(stackAngle);
            z = m_radius * cosf(stackAngle) * sinf(sectorAngle);

            Vertex vertex;
            vertex.position = glm::vec3(x, y, z);
            vertices.push_back(vertex);
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
