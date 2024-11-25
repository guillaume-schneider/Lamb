#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <glm/glm.hpp>
#include <component.hpp>

class Transform : Component {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

#endif
