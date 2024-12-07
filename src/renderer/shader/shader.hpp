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

#endif
