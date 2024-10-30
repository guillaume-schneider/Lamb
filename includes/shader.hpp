#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <glad/glad.h>

struct Shader {
    std::string source;
    unsigned int id;
};

class ShaderFactory {
    public:
        Shader createShader(const std::string& filePath, unsigned int shaderType);
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
};

#endif
