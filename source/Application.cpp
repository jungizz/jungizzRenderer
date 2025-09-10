#include "Application.h"
#include <iostream>

Application::Application(int w, int h, const char* title)
    :width(w), height(h), currentScene(nullptr)
{
    if (!glfwInit()) exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window 창 생성
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);

    glewInit();
}

Application::~Application(){
    if(currentScene) delete currentScene;
    glfwDestroyWindow(window);
    glfwTerminate();
} 

void Application::setScene(Scene* scene){
    if(currentScene) delete currentScene;
    currentScene = scene;
    currentScene->init();

    glfwSetWindowUserPointer(window, currentScene->getCamera());

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y) {
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(win));
    if (cam) cam->cursorPosCallback(win, x, y);
    });

    glfwSetScrollCallback(window, [](GLFWwindow* win, double x, double y) {
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(win));
    if (cam) cam->scrollCallback(win, x, y);
    });
}

void Application::run(){
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::ivec2 framebufferSize;
        glfwGetFramebufferSize(window, &framebufferSize.x, &framebufferSize.y);

        if(currentScene) currentScene->render(framebufferSize);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}