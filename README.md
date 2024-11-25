# LambEngine

## Description

A 3D engine developed in C++ using OpenGL and a set of modern libraries to provide efficient rendering and modular features. This project is designed as a simple yet powerful foundation for experimenting with 3D rendering concepts and ECS systems.

---

## Features

### 🎨 Graphics Rendering
- **MTL File Parser**: Load and manage materials with support for `ambient`, `diffuse`, and `specular` properties.
- **Phong Illumination**: Realistic simulation of reflections and lighting.
- **Lighting Maps**: Advanced management of lighting textures, including diffuse and specular maps.
- **Lighting System**: Supports directional, point, and spotlights with soft edges.
- **Primitive Creation**: Parametric creation of cubes and spheres.

### 📷 Camera Management
- Smooth movement using mouse and keyboard.
- FPS and orbital camera modes.
- Powered by the `glm` library for 3D transformations and calculations.

### 🛠️ ECS (Entity Component System)
- Minimal entity and component management for clear organization of scene objects.
- Easily extendable to add new features like animations or custom behaviors.

### 📂 File Handling
- Load 3D models via **Assimp** (supported formats: `.obj`, `.fbx`, etc.).
- Parse and manage materials from `.mtl` files.

### 🔍 Unit Testing
- Built with **Google Test (gtest)** to ensure engine stability and reliability.

### 🖱️ User Interface
- Integration of **Dear ImGui** for interactive inspection and debugging tools.

---

## Technologies Used

- **OpenGL**: Graphics rendering API.
- **GLM**: Mathematical library for transformations and 3D calculations.
- **SDL2**: Window, input, and event management.
- **Assimp**: 3D model loading and parsing.
- **GLAD**: OpenGL extension loader.
- **Dear ImGui**: Graphical user interface.
- **Google Test**: Unit testing framework.
- **nlohmann-json**: JSON parsing and manipulation for configurations.

---

## Dependencies

The [**vcpkg**](https://github.com/microsoft/vcpkg) package manager is used to handle all external libraries.
