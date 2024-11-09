#include <camera.hpp>
#include <time.hpp>
#include <input.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL.h>

void Camera::computeCursorCameraMovements(int x, int y) {
    yaw += x * m_mouseSensitivity;
    pitch -= y * m_mouseSensitivity;

    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_direction = glm::normalize(direction);
}

void Camera::computeAction(Action action) {
    float deltaTime = Time::getInstance().getDeltaTime();
    switch (action) {
        case Action::Up:
            m_position += m_cameraSpeed * deltaTime * m_up;            
            break;
        case Action::Down:
            m_position -= m_cameraSpeed * deltaTime * m_up;
            break;
        case Action::Right:
            m_position += glm::normalize(glm::cross(m_direction, m_up)) 
                          * m_cameraSpeed * deltaTime;
            break;
        case Action::Left:
            m_position -= glm::normalize(glm::cross(m_direction, m_up))
                          * m_cameraSpeed * deltaTime;
            break;
        case Action::Forward:
            m_position += m_cameraSpeed * deltaTime * m_direction;
            break;
        case Action::Backward:
            m_position -= m_cameraSpeed * deltaTime * m_direction;
            break;
    }
}

void Camera::computeActions(const std::vector<Action> &actions) {
    for (auto action : actions)
        computeAction(action);
}
