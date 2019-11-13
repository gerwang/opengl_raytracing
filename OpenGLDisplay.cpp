//
// Created by gerw on 11/13/19.
//

#include "OpenGLDisplay.h"
#include "Config.h"
#include <iostream>

bool OpenGLDisplay::glInited = false;

void OpenGLDisplay::initGL() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

OpenGLDisplay::OpenGLDisplay() {
    if (!glInited) {
        initGL();
        glInited = true;
    }
    window = glfwCreateWindow(Config::windowWidth, Config::windowHeight, "OpenGL Ray Tracing", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
}
