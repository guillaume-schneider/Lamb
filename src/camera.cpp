#include "camera.hpp"
#include "time.hpp"
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

void Camera::computeCameraMovements() {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    float deltaTime = Time::getInstance().getDeltaTime();
    if (keystate[SDL_SCANCODE_W]) {
        m_position += m_cameraSpeed * deltaTime * m_direction;
    }
    if (keystate[SDL_SCANCODE_S]) {
        m_position -= m_cameraSpeed * deltaTime * m_direction;
    }
    if (keystate[SDL_SCANCODE_A]) {
        m_position -= glm::normalize(glm::cross(m_direction, m_up))
                        * m_cameraSpeed * deltaTime;
    }
    if (keystate[SDL_SCANCODE_D]) {
        m_position += glm::normalize(glm::cross(m_direction, m_up)) 
                        * m_cameraSpeed * deltaTime;
    }
}
