#include <texture.hpp>

std::string toString(TextureType type) {
    switch (type) {
        case TextureType::DIFFUSE: return "texture_diffuse";
        case TextureType::SPECULAR: return "texture_specular";
        default: return "Unknown";
    }
};

TextureType fromString(const std::string& str) {
    static std::unordered_map<std::string, TextureType> strToEnum = {
        {"texture_diffuse", TextureType::DIFFUSE},
        {"texture_specular", TextureType::SPECULAR},
    };

    auto it = strToEnum.find(str);
    if (it != strToEnum.end()) {
        return it->second;
    } else {
        throw std::invalid_argument("Invalid color string");
    }
};
