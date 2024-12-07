#include <shader_engine.hpp>
#include <iostream>


void ShaderEngine::addShader(Shader& shader) {
    const char* sourceCstr = shader.source.c_str();
    glShaderSource(shader.id, 1, &sourceCstr, NULL);
    glCompileShader(shader.id);

    int success;
    char infoLog[512];
    glGetShaderiv(shader.id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader.id, 512, NULL, infoLog);
        std::cerr << "Failed to compile vertex shader: " << infoLog << std::endl;
    } else {
        m_shaders.push_back(shader);
    }
}

void ShaderEngine::compile() {
    m_shaderProgramID = glCreateProgram();

    for (Shader shader : m_shaders) {
        glAttachShader(m_shaderProgramID, shader.id);
    }

    glLinkProgram(m_shaderProgramID);

    int success;
    char infoLog[512];
    glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_shaderProgramID, 512, NULL, infoLog);
        std::cerr << "Failed to link shader program: " << infoLog << std::endl;
    }

    for (Shader shader : m_shaders) {
        glDeleteShader(shader.id);
    }

    m_shaders.clear();
}

void ShaderEngine::use() {
    glUseProgram(m_shaderProgramID);
}
