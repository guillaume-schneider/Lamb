#ifndef INPUT_HPP_
#define INPUT_HPP_

#include <SDL2/SDL.h>
#include <vector>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <mutex>


enum class Action {
    Up,
    Down,
    Left,
    Right,
    Forward,
    Backward
};

inline std::vector<Action> getActions(const Uint8* keystate) {
    std::vector<Action> actions;
    if (keystate[SDL_SCANCODE_W]) actions.push_back(Action::Forward);
    if (keystate[SDL_SCANCODE_S]) actions.push_back(Action::Backward);
    if (keystate[SDL_SCANCODE_A]) actions.push_back(Action::Left);
    if (keystate[SDL_SCANCODE_D]) actions.push_back(Action::Right);
    if (keystate[SDL_SCANCODE_LCTRL]) actions.push_back(Action::Down);
    if (keystate[SDL_SCANCODE_SPACE]) actions.push_back(Action::Up);
    return actions;
};

class InputSystem;

/**
 * @class InputHandler
 * @brief Handles input events such as cursor movement.
 * 
 * This class provides mechanisms to set callbacks for input events and process them.
 */
class InputHandler {
public:
    /**
     * @typedef CursorMovementCallback
     * @brief A callback function type for cursor movement events.
     * 
     * The callback function takes two integer parameters representing the cursor's new position.
     */
    using CursorMovementCallback = std::function<void(int, int)>;

    /**
     * @fn void setCursorMovementCallback(CursorMovementCallback callback)
     * @brief Sets the callback function for cursor movement events.
     * 
     * @param callback The function to be called when the cursor moves.
     */
    void setCursorMovementCallback(CursorMovementCallback callback) {
        m_onCursorMovement = callback;
    }

    /**
     * @fn void processInputs()
     * @brief Processes all input events.
     * 
     * This function should be called to handle and dispatch input events.
     */
    void processInputs() {
        processCursorMovement();
    }

    /**
     * @fn void processCursorMovement()
     * @brief Processes cursor movement events.
     * 
     * This function is called internally by processInputs() to handle cursor movement.
     */
    void processCursorMovement();

private:
    CursorMovementCallback m_onCursorMovement;
};

/**
 * @class InputSystem
 * @brief General input class that monitor input handlers and update the main
 * input pipeline. This class is a Singleton.
 * 
 */
class InputSystem {
public:
    /**
     * @brief Get the Instance object
     * 
     * @return InputSystem* 
     */
    static InputSystem* getInstance() {
        std::lock_guard<std::mutex> lock(mutex);
        if (instance == nullptr) instance = new InputSystem();
        return instance;
    }

    /**
     * @brief Update the main input pipeline.
     * 
     * @param window 
     */
    void update(SDL_Window *window) {
        handleEvents(window);
        for (auto& handler : m_handlers) {
            handler.processInputs();
        }
    }

    /**
     * @brief Add input handler to be updated by the input system.
     * 
     * @param handler 
     */
    void addHandler(const InputHandler& handler) {
        m_handlers.push_back(handler);
    }

    /**
     * @brief Enable relative mouse capture.
     * 
     * @param window 
     */
    void toggleMouseCapture(SDL_Window* window);

    bool isMouseCaptureEnabled() const { return m_isMouseCaptureEnabled; }

    /**
     * @brief Checks if the application or process should stop running.
     * 
     * @return true if the application should stop.
     * @return false if the application should continue running.
     */
    bool shouldStop() const { return m_running; }

private:
    static InputSystem* instance;
    static std::mutex mutex;

    bool m_isMouseCaptureEnabled{true};
    bool m_running{true};
    std::vector<InputHandler> m_handlers;

    InputSystem() = default;

    InputSystem(const InputSystem&) = delete;
    InputSystem& operator=(const InputSystem&) = delete;

    void handleEvents(SDL_Window *window);
};

/**
 * @class InputHandlerFactory
 * @brief Factory class for creating and managing input handlers.
 *
 * This class provides a static method to create an input handler and register it with the input system.
 */
class InputHandlerFactory {
public:
    /**
     * @brief Creates an input handler and registers it with the input system.
     *
     * This static method creates an InputHandler object, sets its cursor movement callback,
     * and adds it to the InputSystem. If the callback is null, an exception is thrown.
     * If adding the handler to the InputSystem fails, an error message is printed and the exception is rethrown.
     *
     * @param callback The cursor movement callback to be set for the input handler.
     * @throws std::invalid_argument if the callback is null.
     * @throws std::exception if adding the handler to the InputSystem fails.
     */
    static void createInputHandler(InputHandler::CursorMovementCallback callback) {
        if (!callback) {
            throw std::invalid_argument("Callback cannot be null.");
        }

        InputHandler handler;
        handler.setCursorMovementCallback(callback);

        try {
            InputSystem::getInstance()->addHandler(std::move(handler));
        } catch (const std::exception& e) {
            std::cerr << "Failed to add handler: " << e.what() << std::endl;
            throw;
        }
    };
};

#endif
