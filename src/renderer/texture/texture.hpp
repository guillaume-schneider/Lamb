#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <glad/glad.h>
#include <string>
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
