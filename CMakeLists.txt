cmake_minimum_required(VERSION 3.23)

project(LambEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

file(GLOB SOURCES
    "src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE includes)

# Find and link libraries
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)

find_package(Stb REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

add_definitions(-DDEBUG)

# Optionally add other settings, such as compiler flags, etc.
# target_compile_options(GameEngine PRIVATE -Wall -Wextra)
