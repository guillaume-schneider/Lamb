#ifndef ENTITY_MANAGER_HPP_
#define ENTITY_MANAGER_HPP_

#include <vector>
#include <algorithm>
#include "entity.hpp"

class EntityManager {
public:
    static EntityManager& getInstance() {
        static EntityManager instance;
        return instance;
    }

    void addEntity(Entity* entity) {
        m_entities.push_back(entity);
    }

    void removeEntity(Entity* entity) {
        m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
    }

    void removeEntity(int id) {
        m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(),
            [id](Entity* entity) { return entity->getId() == id; }), m_entities.end());
    }

    Entity* getEntity(int id) const {
        for (auto& entity : m_entities) {
            if (entity->getId() == id) return entity;
        }
        return nullptr;
    }

    std::vector<Entity*> getEntities() const {
        return m_entities;
    }

private:
    std::vector<Entity*> m_entities;
    EntityManager() {}
    ~EntityManager() {}
    EntityManager& operator=(EntityManager&) = delete;
    EntityManager(const EntityManager&) = delete;
};

class EntityFactory {
public:
    static Entity createEntity() {
        Entity entity;
        EntityManager::getInstance().addEntity(&entity);
        return entity;
    }
};

#endif
