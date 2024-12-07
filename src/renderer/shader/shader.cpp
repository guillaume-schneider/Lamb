#include "shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <string>


Shader ShaderFactory::createShader(const std::string& filePath, unsigned int shaderType) {
    Shader shader;
    shader.id = glCreateShader(shaderType);

    // Read the shader file
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return shader;
    }

    std::stringstream shaderStream;
    shaderStream << fileStream.rdbuf();
    shader.source = shaderStream.str();

    return shader;
}
