//
// Created by gerw on 11/13/19.
//

#include "OpenGLDisplay.h"
#include "Config.h"
#include <iostream>
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

bool OpenGLDisplay::glInited = false;

void OpenGLDisplay::initGL() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glInited = true;
}

OpenGLDisplay::OpenGLDisplay() {
    if (!glInited) {
        initGL();
    }
    window = glfwCreateWindow(Config::windowWidth, Config::windowHeight, "OpenGL Ray Tracing", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char *glslVersion = "#version 430";
    ImGui_ImplOpenGL3_Init(glslVersion);

}

void OpenGLDisplay::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        imGuiStart();

        int displayW, displayH;
        glfwGetFramebufferSize(window, &displayW, &displayH);
        glViewport(0, 0, displayW, displayH);

        imGuiEnd();
    }
}

void OpenGLDisplay::imGuiStart() {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        ImGui::Begin("View Control");

        ImGui::End();
    }
}

void OpenGLDisplay::imGuiEnd() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

OpenGLDisplay::~OpenGLDisplay() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    glInited = false;
}
