#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Shader {
    std::string source;
    unsigned int id;
};

class ShaderFactory {
    public:
        static Shader createShader(const std::string& filePath, unsigned int shaderType);
};

class ShaderEngine {
    private:
        std::vector<Shader> m_shaders;
        unsigned int m_shaderProgramID;
    public:
        void addShader(Shader& shader);
        void compile();

        unsigned int getShaderProgramID() { return m_shaderProgramID; };
        void use();
        void setInt(const std::string& name, int value) { 
            glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), value); 
        };
        void setVec3(const std::string& name, float x, float y, float z) {
            glUniform3f(glGetUniformLocation(m_shaderProgramID, name.c_str()),
                        x, y, z);
        };
        void setVec3(const std::string& name, glm::vec3 value) {
            ShaderEngine::setVec3(name, value.x, value.y, value.z);
        }
        void setMat4(const std::string& name, glm::mat4 mat) {
            glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, name.c_str()),
                1, GL_FALSE, glm::value_ptr(mat));
        }

        void setFloat(const std::string& name, float value) {
            glUniform1f(glGetUniformLocation(m_shaderProgramID, name.c_str()),
                value);
        }

        int size() {
            return m_shaders.size();
        }
};

#endif
