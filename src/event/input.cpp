#include <input.hpp>
#include <SDL2/SDL.h>
#include <imgui_impl_sdl2.h>


InputSystem* InputSystem::instance = nullptr;
std::mutex InputSystem::mutex;

void InputSystem::toggleMouseCapture(SDL_Window* window) {
    if (m_isMouseCaptureEnabled) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    } else {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        int centerX = windowWidth / 2;
        int centerY = windowHeight / 2;

        SDL_WarpMouseInWindow(window, centerX, centerY);
    }
};

void InputSystem::handleEvents(SDL_Window* window) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            m_running = false;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            m_isMouseCaptureEnabled = !m_isMouseCaptureEnabled;
            toggleMouseCapture(window);
        }
    }
};

void InputHandler::processCursorMovement() {
    if (m_onCursorMovement) {
        int xrel, yrel;
        SDL_GetRelativeMouseState(&xrel, &yrel);
        if (InputSystem::getInstance()->isMouseCaptureEnabled()) {
            if (xrel != 0 || yrel != 0) {
                m_onCursorMovement(xrel, yrel);
            }
        }
    }
}
