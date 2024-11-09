#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <input.hpp>
#include <vector>

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 1.0f, 3.0f),
           glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float mouseSensitivity = 0.1f, float cameraSpeed = 2.5f)
        : m_position(position), m_direction(direction), m_up(up), 
          m_mouseSensitivity(mouseSensitivity), m_cameraSpeed(cameraSpeed) {};

    void computeCursorCameraMovements(int, int);

    void computeActions(const std::vector<Action> &actions);

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(m_position, m_position + m_direction, m_up);
    };

    glm::vec3 getPosition() const {
        return m_position;
    }

private:
    glm::vec3 m_position;       // Camera position
    glm::vec3 m_direction;      // Camera direction
    glm::vec3 m_up;             // Up vector
    float yaw = -90.0f;           // Yaw angle
    float pitch = 0.0f;         // Pitch angle
    float m_mouseSensitivity;   // Sensitivity for mouse movement
    float m_cameraSpeed;        // Speed of the camera movement

    void computeAction(Action action);
};

#endif
