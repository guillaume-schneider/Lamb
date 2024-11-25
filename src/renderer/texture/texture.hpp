#ifndef TEXTURE_HPP_H
#define TEXTURE_HPP_H

#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <stdexcept>

enum TextureType {
    NOT_TEXTURE = -1,
    DIFFUSE,
    SPECULAR
};

std::string toString(TextureType type);
TextureType fromString(const std::string& str);

struct Texture {
    GLuint id;
    TextureType type;
    std::string path;
};

#endif
