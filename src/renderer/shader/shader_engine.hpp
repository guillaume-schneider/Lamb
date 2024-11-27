#ifndef SHADER_ENGINE_HPP_
#define SHADER_ENGINE_HPP_

#include <vector>
#include <shader.hpp>

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


class ShaderEngineFactory {
public:
    static ShaderEngine createEngine(const std::string& vertex, const std::string& fragment) {
        ShaderEngine engine;
        Shader vertexShader = ShaderFactory::createShader(vertex, GL_VERTEX_SHADER);
        Shader fragmentShader = ShaderFactory::createShader(fragment, GL_FRAGMENT_SHADER);
        engine.addShader(vertexShader);
        engine.addShader(fragmentShader);
        engine.compile();
        return engine;
    }
};

#endif
