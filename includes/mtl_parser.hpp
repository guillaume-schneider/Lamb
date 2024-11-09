#ifndef MTL_PARSER_H_
#define MTL_PARSER_H_

#include <material.hpp>
#include <unordered_map>
#include <string>

namespace IO {
    std::unordered_map<MaterialType, Material> parseMTL(const std::string& path);
}

#endif
