#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "shader.hpp"
#include "camera.hpp"
#include "time.hpp"
#include "model.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

bool running = true;

void handleEvents(SDL_Event& event) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
};

void APIENTRY openglDebugCallback(GLenum source, GLenum type, GLuint id,
                                  GLenum severity, GLsizei length,
                                  const GLchar* message, const void* userParam) {
    std::cerr << "OpenGL Debug Message: " << message << std::endl;
}

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    SDL_Window* window = SDL_CreateWindow(
        "OpenGL Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    if (gladLoadGL()) {  // Ensure GLAD loaded OpenGL
        GLint flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {  // Check if debug context is active
            std::cout << "GL_CONTEXT::DEBUG::ACTIVATED" << std::endl;
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(openglDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
    } else {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
    }

    const GLubyte* version = glGetString(GL_VERSION);
    if (version) {
        std::cout << "OpenGL Version: " << version << std::endl;
    } else {
        std::cerr << "Unable to retrieve OpenGL version." << std::endl;
    }


    glViewport(0, 0, 800, 600);

    ShaderEngine shaderEngineLighting;
    ShaderEngine shaderEngineLight;

    ShaderFactory shaderFactory;

    Shader lightingVertexShader = shaderFactory.createShader("C:\\Users\\NULL\\Documents\\Games\\GameEngine\\shaders\\lighting_vertex.glsl", GL_VERTEX_SHADER);
    shaderEngineLighting.addShader(lightingVertexShader);
    Shader lightingFragmentShader = shaderFactory.createShader("C:\\Users\\NULL\\Documents\\Games\\GameEngine\\shaders\\lighting_fragment.glsl", GL_FRAGMENT_SHADER);
    shaderEngineLighting.addShader(lightingFragmentShader);
    shaderEngineLighting.compile();

    Shader lightVertexShader = shaderFactory.createShader("C:\\Users\\NULL\\Documents\\Games\\GameEngine\\shaders\\light_vertex.glsl", GL_VERTEX_SHADER);
    shaderEngineLight.addShader(lightVertexShader);
    Shader lightFragmentShader = shaderFactory.createShader("C:\\Users\\NULL\\Documents\\Games\\GameEngine\\shaders\\light_fragment.glsl", GL_FRAGMENT_SHADER);
    shaderEngineLight.addShader(lightFragmentShader);
    shaderEngineLight.compile();

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };

    unsigned int VBO, lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    // set the vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind the VAO

    glEnable(GL_DEPTH_TEST);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    Model model("C:\\Users\\NULL\\Documents\\Games\\GameEngine\\res\\teapot.fbx");

    Camera camera;
    // Main loop
    while (running) {
        Time::getInstance().computeDeltaTime();

        SDL_Event event;
        handleEvents(event);
        camera.computeCameraMovements();

        int xrel, yrel;
        SDL_GetRelativeMouseState(&xrel, &yrel);
        if (xrel != 0 || yrel != 0) {
            camera.computeCursorCameraMovements(xrel, yrel);
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        // modelMatrix = glm::rotate(modelMatrix, (float)SDL_GetTicks64()/128 * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view;
        view = camera.getViewMatrix();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture);

        shaderEngineLighting.use();
        shaderEngineLighting.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        shaderEngineLighting.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        shaderEngineLighting.setMat4("model", modelMatrix);
        shaderEngineLighting.setMat4("view", view);
        shaderEngineLighting.setMat4("projection", projection);
        model.draw(shaderEngineLighting);
        glBindVertexArray(0);

        shaderEngineLight.use();
        shaderEngineLight.setMat4("projection", projection);
        shaderEngineLight.setMat4("view", view);
        modelMatrix = glm::translate(modelMatrix, lightPos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f)); // a smaller cube
        shaderEngineLight.setMat4("model", modelMatrix);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
