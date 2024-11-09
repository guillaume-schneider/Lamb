#include <gtest/gtest.h>
#include <SDL2/SDL.h>
#include "input.hpp"

class InputSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
        inputSystem = InputSystem::getInstance();
    }

    void TearDown() override {
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    SDL_Window* window;
    InputSystem* inputSystem;
};

TEST_F(InputSystemTest, SingletonTest) {
    InputSystem* secondInstance = InputSystem::getInstance();
    ASSERT_EQ(inputSystem, secondInstance);  // Verify both instances are the same
}

TEST_F(InputSystemTest, UpdateTest) {
    InputHandler handler;
    inputSystem->addHandler(handler);

    // Simulate an SDL_QUIT event to stop the input system
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);

    inputSystem->update(window);
    ASSERT_TRUE(inputSystem->shouldStop());  // Should stop after receiving SDL_QUIT event
}
