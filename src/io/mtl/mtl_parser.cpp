#include <mtl_parser.hpp>
#include <material.hpp>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace IO {

    std::unordered_map<MaterialType, Material> parseMTL(const std::string& path) {    
        std::ifstream file(path);
        if (!file) {
            std::cerr << "Error when opening MTL file: " << path << std::endl;
            throw std::runtime_error("File not found: " + path);
        }

        std::unordered_map<MaterialType, Material> materials;
        std::string line;
        Material* material = nullptr;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string identifier;
            iss >> identifier;

            if (identifier == "newmtl") {
                std::string typeString;
                iss >> typeString;
                MaterialType type = Material::getMaterialType(typeString);
                
                material = &materials[type];
                material->type = type;
            } else if (material) {
                if (identifier == "#" || iss.str().size() == 0)
                    continue;
                switch (identifier[0]) {
                    case 'K':
                        if (identifier == "Ka") {
                            iss >> material->ambient[0] >> material->ambient[1] >> material->ambient[2];
                        } else if (identifier == "Kd") {
                            iss >> material->diffuse[0] >> material->diffuse[1] >> material->diffuse[2];
                        } else if (identifier == "Ks") {
                            iss >> material->specular[0] >> material->specular[1] >> material->specular[2];
                        }
                        break;
                    case 'N':
                        if (identifier == "Ns") {
                            iss >> material->shininess;
                        }
                        break;
                    case 'd':
                        break;
                    default:
                        std::cerr << "Unexpected identifier: " << identifier << std::endl;
                }
            } else {
                std::cerr << "Warning: No material defined yet, line: " << line << std::endl;
            }
        }
        
        return materials;
    }

}
