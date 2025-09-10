#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Scene.h"
#include "core/Camera.h"

class Application{
public:
    Application(int width, int height, const char* title);
    ~Application();
    
    void run();
    void setScene(Scene* scene);

private:
    GLFWwindow* window;
    Scene* currentScene;
    int width, height;
};