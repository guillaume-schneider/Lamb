cmake_minimum_required (VERSION 3.23)

project(GameEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(out src/main.cpp)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(out PRIVATE SDL2::SDL2main SDL2::SDL2)

find_package(glad CONFIG REQUIRED)
target_link_libraries(out PRIVATE glad::glad)

find_package(OpenGL REQUIRED)
target_link_libraries(out PRIVATE OpenGL::GL)
