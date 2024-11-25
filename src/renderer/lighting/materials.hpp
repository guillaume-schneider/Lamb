#ifndef MATERIALS_H_
#define MATERIALS_H_

#include <material.hpp>
#include <mtl_parser.hpp>
#include <config_manager.hpp>

class MaterialManager {
public:
    static MaterialManager* getInstance() {
        if (instance == nullptr) instance = new MaterialManager();
        return instance;
    }

    Material getMaterial(MaterialType type) {
        return m_materials[type];
    }

private:
    static MaterialManager* instance;
    std::unordered_map<MaterialType, Material> m_materials;

    MaterialManager(const MaterialManager&) = delete;
    MaterialManager& operator=(const MaterialManager) = delete;
    MaterialManager() {
        m_materials = IO::parseMTL(ConfigurationManager::getInstance()->getMaterialsPath());
    };
    ~MaterialManager() {};
};

#endif
