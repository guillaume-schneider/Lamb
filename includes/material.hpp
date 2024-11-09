#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <shader.hpp>


enum class MaterialType {
    NOT_MATERIAL = -1,
    SILVER,
    GOLD
};

struct Material {
    Material()
        : ambient(glm::vec3(0.0f)), diffuse(glm::vec3(0.0f)), specular(glm::vec3(0.0f)),
          type(MaterialType::NOT_MATERIAL), shininess(0.0f), vertex(nullptr), fragment(nullptr) {}

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess,
             const Shader* vertex, const Shader* fragment)
        : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess),
          type(MaterialType::NOT_MATERIAL), vertex(vertex), fragment(fragment) {}

    MaterialType type = MaterialType::NOT_MATERIAL;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    const Shader* vertex;
    const Shader* fragment;

    static MaterialType getMaterialType(const std::string& mtl_type) {
        if (mtl_type == "Silver") {
            return MaterialType::SILVER;
        } else if (mtl_type == "Gold") {
            return MaterialType::GOLD;
        } else {
            return MaterialType::NOT_MATERIAL;
        }
    }

    static std::string getMaterialString(MaterialType type) {
        switch (type) {
            case MaterialType::GOLD:
                return "Gold";
            case MaterialType::SILVER:
                return "Silver";
            default:
                return "Not material";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Material& material) {
        os << "Material(type: " << Material::getMaterialString(material.type)
           << ", ambient: [" << material.ambient[0] << ", " << material.ambient[1] << ", " << material.ambient[2] << "]"
           << ", diffuse: [" << material.diffuse[0] << ", " << material.diffuse[1] << ", " << material.diffuse[2] << "]"
           << ", specular: [" << material.specular[0] << ", " << material.specular[1] << ", " << material.specular[2] << "])";
        return os;
    }
};

#endif
