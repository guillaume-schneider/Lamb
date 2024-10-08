cmake_minimum_required(VERSION 3.23)

project(GameEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(SOURCES
    src/main.cpp
    src/shader.cpp)

# Add executable
add_executable(GameEngine ${SOURCES})

# Find and link libraries
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(GameEngine PRIVATE SDL2::SDL2main SDL2::SDL2)

find_package(glad CONFIG REQUIRED)
target_link_libraries(GameEngine PRIVATE glad::glad)

find_package(OpenGL REQUIRED)
target_link_libraries(GameEngine PRIVATE OpenGL::GL)

# Optionally add other settings, such as compiler flags, etc.
# target_compile_options(GameEngine PRIVATE -Wall -Wextra)
