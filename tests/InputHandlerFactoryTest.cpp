#include <gtest/gtest.h>
#include <stdexcept>
#include "input.hpp"

TEST(InputHandlerFactoryTest, CreateInputHandlerTest) {
    // Test valid callback
    bool callbackCalled = false;
    auto validCallback = [&](int xrel, int yrel) {
        callbackCalled = true;
    };
    EXPECT_NO_THROW(InputHandlerFactory::createInputHandler(validCallback));
    ASSERT_TRUE(callbackCalled);  // Callback should have been set correctly

    // Test invalid callback (null callback)
    EXPECT_THROW(InputHandlerFactory::createInputHandler(nullptr), std::invalid_argument);
}
