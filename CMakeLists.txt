cmake_minimum_required(VERSION 3.23)

project(LambEngine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "includes/*.hpp" "includes/*.h")

option(BUILD_TESTS "Build tests" OFF)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Build type-specific settings
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(BUILD_TESTS OFF)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /GL /DNDEBUG")
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} /MT /EHsc")
    else()
        set(CMAKE_CXX_FLAGS "-O3 /O2 /DNDEBUG")
    endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_TESTS ON)
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "/Od /Zi /D_DEBUG /EHsc")
else()
    message(WARNING "CMAKE_BUILD_TYPE is not set to Debug or Release, defaulting to Debug.")
    set(BUILD_TESTS ON)
endif()

# Add test-related subdirectory if BUILD_TESTS is ON
if (BUILD_TESTS)
    message(STATUS "Building in development mode with tests.")
    enable_testing()
    add_subdirectory(tests)
else()
    message(STATUS "Building in production mode (no tests).")
endif()

# Main executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories for the main project
target_include_directories(${PROJECT_NAME} PRIVATE "includes")

# Link libraries for the main project
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

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
