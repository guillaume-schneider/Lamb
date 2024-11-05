#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "shader.hpp"
#include "camera.hpp"
#include "time.hpp"
#include "model.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr unsigned int WINDOW_WIDTH = 1980;
constexpr unsigned int WINDOW_HEIGHT = 1080;

int lastX, lastY;
bool running = true;
bool enableMouseCapture = true;

std::vector<int> components;

void toggleMouseCapture(SDL_Window* window) {
    if (enableMouseCapture) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    } else {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        int centerX = windowWidth / 2;
        int centerY = windowHeight / 2;

        SDL_WarpMouseInWindow(window, centerX, centerY);
    }
}

void handleEvents(SDL_Event& event, SDL_Window* window) {
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            enableMouseCapture = !enableMouseCapture;
            toggleMouseCapture(window);
        }
    }
};

std::vector<float> computeVertexSphere(int sectorCount, int stackCount, int radius) {
    float stackAngle, sectorAngle;
    float x, y, z;

    std::vector<float> vertices;
    for (int i = 0; i < stackCount; i ++){

        stackAngle = M_PI * float(i) / float(stackCount); // angle entre 0 et M_PI

        for (int j = 0; j < sectorCount; j ++) {
            sectorAngle = 2 * M_PI * float(j) / float(sectorCount);

            x = radius * cos(sectorAngle) * cos(stackAngle);
            y = radius * sin(sectorAngle) * cos(stackAngle);
            z = radius * sin(stackAngle);

            // Vertex vertex;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    return vertices;
}

std::vector<int> computeIndicesSpheres(int stackCount, int sectorCount) {
    std::vector<int> indices;
    int k1, k2;
    for (int i = 0; i < stackCount; i++) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;
        for (int j = 0; j < sectorCount; j++) {

            if (i != 0) {
                indices.push_back(k1 + j);
                indices.push_back(k2 + j);
                indices.push_back(k1 + j + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + j + 1);
                indices.push_back(k2 + j);
                indices.push_back(k2 + j + 1);
            }
        }
    }
    return indices;
}



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

#ifdef DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    SDL_Window* window = SDL_CreateWindow(
        "OpenGL Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH, WINDOW_HEIGHT,
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    
    if (SDL_GL_MakeCurrent(window, context) != 0) {
        std::cerr << "SDL_GL_MakeCurrent::ERROR::" << SDL_GetError() << std::endl;
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

#ifdef VSYNC
    SDL_GL_SetSwapInterval(1);
#endif

#ifdef DEBUG
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
#endif

    const GLubyte* version = glGetString(GL_VERSION);
    if (version) {
        std::cout << "OpenGL Version: " << version << std::endl;
    } else {
        std::cerr << "Unable to retrieve OpenGL version." << std::endl;
    }

    int currentWindowWidth, currentWindowHeight;
    SDL_GetWindowSize(window, &currentWindowWidth, &currentWindowHeight);
    float currrentWindowRatio = static_cast<float>(currentWindowWidth) / currentWindowHeight;

    glViewport(0, 0, currentWindowWidth, currentWindowHeight);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init();


    ShaderEngine shaderEngineLighting;
    ShaderEngine shaderEngineLight;

    ShaderFactory shaderFactory;

    Shader lightingVertexShader = shaderFactory.createShader("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\shaders\\lighting_vertex.glsl", GL_VERTEX_SHADER);
    shaderEngineLighting.addShader(lightingVertexShader);
    Shader lightingFragmentShader = shaderFactory.createShader("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\shaders\\lighting_fragment.glsl", GL_FRAGMENT_SHADER);
    shaderEngineLighting.addShader(lightingFragmentShader);
    shaderEngineLighting.compile();

    Shader lightVertexShader = shaderFactory.createShader("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\shaders\\light_vertex.glsl", GL_VERTEX_SHADER);
    shaderEngineLight.addShader(lightVertexShader);
    Shader lightFragmentShader = shaderFactory.createShader("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\shaders\\light_fragment.glsl", GL_FRAGMENT_SHADER);
    shaderEngineLight.addShader(lightFragmentShader);
    shaderEngineLight.compile();

    ShaderEngine shaderEngineBasic;
    Shader basicVertexShader = shaderFactory.createShader("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\shaders\\basic_vertex.glsl", GL_VERTEX_SHADER);
    shaderEngineBasic.addShader(basicVertexShader);
    Shader basicFragmentShader = shaderFactory.createShader("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\shaders\\basic_fragment.glsl", GL_FRAGMENT_SHADER);
    shaderEngineBasic.addShader(basicFragmentShader);
    shaderEngineBasic.compile();

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

    Model model("C:\\Users\\NULL\\Documents\\Games\\LambEngine\\res\\teapot.fbx");

    Camera camera;

    std::vector<float> sphereVertices = computeVertexSphere(16, 36, 5);
    std::vector<int> sphereIndices = computeIndicesSpheres(16, 32);

    unsigned int VBOSphere, EBOSphere, VAOSphere;
    glGenVertexArrays(1, &VAOSphere);
    glGenBuffers(1, &VBOSphere);
    glGenBuffers(1, &EBOSphere);

    glBindVertexArray(VAOSphere);
    glBindBuffer(GL_ARRAY_BUFFER, VBOSphere);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOSphere);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

    glBindVertexArray(0);

    // Main loop
    while (running) {
        Time::getInstance().computeDeltaTime();

        SDL_Event event;
        handleEvents(event, window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Handle platform windows for multi-viewports
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        camera.computeCameraMovements();

        int xrel, yrel;
        SDL_GetRelativeMouseState(&xrel, &yrel);
        if (enableMouseCapture) {
            if (xrel != 0 || yrel != 0) {
                camera.computeCursorCameraMovements(xrel, yrel);
            }
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix, (float)SDL_GetTicks()/256, glm::vec3(0.0f, 1.0f, 0.0f));
        // modelMatrix = glm::rotate(modelMatrix, (float)SDL_GetTicks64()/128 * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view(1.0f);
        view = camera.getViewMatrix();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f),
            currrentWindowRatio, 0.1f, 100.0f);

        // shaderEngineLight.use();
        // shaderEngineLight.setMat4("projection", projection);
        // shaderEngineLight.setMat4("view", view);
        // modelMatrix = glm::translate(modelMatrix, lightPos);
        // glm::vec3 lightPosition = glm::vec3(modelMatrix[3]);

        // modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
        // shaderEngineLight.setMat4("model", modelMatrix);

        // glBindVertexArray(lightVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0);

        // modelMatrix = glm::mat4(1.0f);

        // shaderEngineLighting.use();
        // shaderEngineLighting.setVec3("objectColor", 1.0f, 0.5f, 0.31f);

        // shaderEngineLighting.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        // shaderEngineLighting.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        // shaderEngineLighting.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        // shaderEngineLighting.setFloat("material.shininess", 32.0f);

        // shaderEngineLighting.setVec3("light.position", lightPosition.x,
        //     lightPosition.y, lightPosition.z);

        // glm::vec3 lightColor;
        // lightColor.x = sin(SDL_GetTicks64()/1048 * 2.0f);
        // lightColor.y = sin(SDL_GetTicks64()/1048 * 0.7f);
        // lightColor.z = sin(SDL_GetTicks64()/1048 * 1.3f);
        
        // glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); 
        // glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); 

        // shaderEngineLighting.setVec3("light.ambient",  ambientColor);
        // shaderEngineLighting.setVec3("light.diffuse",  diffuseColor);
        // shaderEngineLighting.setVec3("light.specular", 1.0f, 1.0f, 1.0f); 

        // shaderEngineLighting.setVec3("cameraPosition", camera.getPosition());

        // shaderEngineLighting.setMat4("model", modelMatrix);
        // shaderEngineLighting.setMat4("view", view);
        // shaderEngineLighting.setMat4("projection", projection);
        // model.draw(shaderEngineLighting);
        // glBindVertexArray(0);

        shaderEngineBasic.use();
        shaderEngineBasic.setMat4("model", modelMatrix);
        shaderEngineBasic.setMat4("view", view);
        shaderEngineBasic.setMat4("projection", projection);

        glBindVertexArray(VAOSphere);
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
