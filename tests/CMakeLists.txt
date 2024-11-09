# tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.23)

# Add the main project's include directories so the tests can find the headers
include_directories(${CMAKE_SOURCE_DIR}/includes)

# Add the source files for the tests
file(GLOB TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cpp")

if (TEST_SOURCES)
    find_package(GTest CONFIG REQUIRED)

    # Create the tests executable
    add_executable(Tests ${TEST_SOURCES})

    # Link the necessary libraries to the tests executable
    target_link_libraries(Tests PRIVATE
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
    )

    add_test(NAME AllTests COMMAND Tests)
else()
    message(STATUS "No test files found in the 'tests' directory.")
endif()
