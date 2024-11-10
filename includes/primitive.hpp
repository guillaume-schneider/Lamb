#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_


#include <vector>
#include <glad/glad.h>
#include <sstream>


class Primitive {
public:    
    Primitive() : m_VAO(0), m_VBO(0), m_EBO(0) {}
    virtual ~Primitive() {
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        glDeleteVertexArrays(1, &m_VAO);
    }

    void draw();
    std::vector<float> getVertices() { return m_vertices; }
    std::vector<unsigned int> getIndices() { return m_indices; }
protected:
    GLuint m_VAO, m_VBO, m_EBO;
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;

    void init() {
        m_vertices = computeVertices();
        m_indices = computeIndices();
        setupDrawing();
    }
    virtual std::vector<float> computeVertices() = 0;
    virtual std::vector<unsigned int> computeIndices() = 0;
    void setupDrawing();
};

class Cube : public Primitive {
public:
    Cube() : Primitive(), m_scale(1.0f) { init(); }
    Cube(float scale) {
        m_scale = scale;
        init();
    }

    void setScale(float value) { 
        m_scale = value;
        computeVertices();
    }
    float getScale() { return m_scale; }

    friend std::ostream& operator<<(std::ostream& os, const Cube& cube) {
        os << "Cube Scale: " << cube.m_scale << "\n";

        // Print vertices
        os << "Vertices: ";
        for (size_t i = 0; i < cube.m_vertices.size(); i += 3) {
            os << "(" << cube.m_vertices[i] << ", "
               << cube.m_vertices[i + 1] << ", "
               << cube.m_vertices[i + 2] << ") ";
        }
        os << "\n";

        // Print indices
        os << "Indices: ";
        for (size_t i = 0; i < cube.m_indices.size(); ++i) {
            os << cube.m_indices[i] << " ";
        }
        os << "\n";

        return os;
    }
protected:
    std::vector<float> computeVertices() override;
    std::vector<unsigned int> computeIndices() override;
private:
    float m_scale{1.0f};
};

class Sphere : public Primitive {
public:
    Sphere() : Primitive(), m_radius(1.0f), m_stackCount(16), m_sectorCount(32)
        { init(); }
    Sphere(float radius) : Sphere() {
        m_radius = radius;
    } 
    Sphere(int stackCount, int sectorCount, float radius) : Sphere(radius) {
        m_stackCount = stackCount;
        m_sectorCount = sectorCount;
    } 
    void setStackCount(int value) { m_stackCount = value; }
    void setSectorCount(int value) { m_sectorCount = value; }
    void setRadius(float value) { m_radius = value; }
    std::vector<float> computeVertices() override;
    std::vector<unsigned int> computeIndices() override;
private:
    int m_stackCount{16};
    int m_sectorCount{32};
    float m_radius{1.0f};
};

#endif
