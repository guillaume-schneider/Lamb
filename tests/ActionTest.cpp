#include <gtest/gtest.h>
#include <SDL2/SDL.h>
#include <vector>
#include <input.hpp>

TEST(ActionTest, GetActionsTest) {
    Uint8 keystate[SDL_NUM_SCANCODES] = {0};

    // Test for the "W" key (Forward)
    keystate[SDL_SCANCODE_W] = 1;
    auto actions = getActions(keystate);
    ASSERT_EQ(actions.size(), 1);
    ASSERT_EQ(actions[0], Action::Forward);

    // Test for the "S" key (Backward)
    keystate[SDL_SCANCODE_W] = 0;
    keystate[SDL_SCANCODE_S] = 1;
    actions = getActions(keystate);
    ASSERT_EQ(actions.size(), 1);
    ASSERT_EQ(actions[0], Action::Backward);

    // Test for multiple keys
    keystate[SDL_SCANCODE_W] = 1;
    keystate[SDL_SCANCODE_A] = 1;
    actions = getActions(keystate);
    ASSERT_EQ(actions.size(), 2);
    ASSERT_EQ(actions[0], Action::Forward);
    ASSERT_EQ(actions[1], Action::Left);
}
