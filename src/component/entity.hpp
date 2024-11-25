#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <transform.hpp>
#include <model.hpp>

class Entity {
public:
    Entity() : id(entityCount++) {}
    ~Entity() {}

    int getId() const { return id; }

    Transform transform;
    // Model model;
private:
    int id;
    static int entityCount;
};

#endif
