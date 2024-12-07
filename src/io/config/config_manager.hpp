#ifndef CONFIG_MATERIAL_H_
#define CONFIG_MATERIAL_H_

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

class ConfigurationManager {
public:
    static ConfigurationManager* getInstance() {
        if (instance == nullptr) instance = new ConfigurationManager();
        return instance;
    }

    std::string getMaterialsPath() {
        return m_materials_path;
    }

private:
    static ConfigurationManager* instance;
    json m_config;
    std::string m_materials_path;
    const char* m_config_path = ".\\config\\links.json";

    ConfigurationManager(const ConfigurationManager&) = delete;
    ConfigurationManager& operator=(const ConfigurationManager) = delete;

    json parseJSON(const std::string& path) {
        std::ifstream inputFile(path);
        if (!inputFile.is_open()) {
            std::cerr << "Error opening file: " << path << std::endl;
            return nullptr;
         }

        json buffer;
        inputFile >> buffer;
        return buffer;
    }

    ConfigurationManager() {
        m_config = parseJSON(m_config_path);
        m_materials_path = m_config["materials"];
    }
    ~ConfigurationManager() {}
};

#endif
