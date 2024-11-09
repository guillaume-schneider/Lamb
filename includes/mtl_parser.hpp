#ifndef MTL_PARSER_H_
#define MTL_PARSER_H_

#include <unordered_map>
#include <string>
#include <material.hpp>

namespace IO {
    std::unordered_map<MaterialType, Material> parseMTL(const std::string& path);
};

#endif
