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
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos) {
        auto *me = (OpenGLDisplay *) glfwGetWindowUserPointer(window);
        me->mouseMoveCallback(window, xpos, ypos);
    });

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char *glslVersion = "#version 430";
    ImGui_ImplOpenGL3_Init(glslVersion);

    // load scene
    std::string scenePath = Config::assetRoot + "/scenes/MainScene.json";
    scene = std::make_unique<Scene>(scenePath);
    scene->loadAssets(assetManager);
    scene->initCamera(camera);


    shader = std::make_unique<Shader>((Config::assetRoot + "/shaders/vertex_shader.glsl").c_str(),
                                      (Config::assetRoot + "/shaders/fragment_shader.glsl").c_str());
}

void OpenGLDisplay::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        processKeyInput();
        if (showUI) {
            imGuiStart();
        }

        int displayW, displayH;
        glfwGetFramebufferSize(window, &displayW, &displayH);
        glViewport(0, 0, displayW, displayH);

        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projMat = camera.getProjectMat();
        glm::mat4 viewMat = camera.getViewMat();
        shader->use();
        shader->setInt("textureMap", 0);
        shader->setInt("normalMap", 1);
        shader->setVec3("viewPos", camera.pos);
        for (auto &mesh:scene->meshRefs) {
            mesh.modelMat = glm::mat4(1.0f); // fixme
            mesh.modelMat[3][2] = -2.0f;
            glm::mat4 mvMat = viewMat * mesh.modelMat;
            glm::mat4 mvpMat = projMat * mvMat;
            glm::mat4 normalMat = glm::transpose(glm::inverse(mvMat));
            shader->setMat4("mvpMat", mvpMat);
            shader->setMat4("normalMat", normalMat);
            shader->setMat4("modelMat", mesh.modelMat);
            assetManager.textureMap[AssetManager::COLOR][mesh.texture]->use(0);
            shader->setVec3("lightPos", scene->lights[0].pos);
            shader->setVec3("lightColor", scene->lights[0].color);
            assetManager.textureMap[AssetManager::NORMAL][mesh.normal]->use(1);
            assetManager.meshMap[mesh.ply]->draw();
        }
        shader->noUse();

        glDisable(GL_DEPTH_TEST);

        if (showUI) {
            imGuiEnd();
        }
        glfwSwapBuffers(window);
    }
}

void OpenGLDisplay::imGuiStart() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        ImGui::Begin("View Control");
        ImGui::Checkbox("Use perspective or orthogonal", &camera.usePerspective);
        ImGui::LabelText("camera pos", "%f %f %f", camera.pos.x, camera.pos.y, camera.pos.z);
        ImGui::LabelText("pitch & yaw", "%f %f", camera.pitch, camera.yaw);
        ImGui::End();
    }
}

void OpenGLDisplay::imGuiEnd() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

OpenGLDisplay::~OpenGLDisplay() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    glInited = false;
}

void OpenGLDisplay::processKeyInput() {
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        showUI = !showUI;
        if (showUI) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true;
        }
    }
    if (!showUI) {
        glm::vec3 right = glm::normalize(glm::cross(camera.forward, camera.up));
        glm::vec3 delta = glm::vec3(0.0f);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            delta += right * camera.speed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            delta -= right * camera.speed;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            delta += camera.forward * camera.speed;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            delta -= camera.forward * camera.speed;
        }
        delta = scene->clipMove(camera.pos, delta);
        camera.pos += delta;
    }
}

void OpenGLDisplay::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    if (showUI || firstMouse) {
        lastX = float(xpos);
        lastY = float(ypos);
        firstMouse = false;
    }
    float xOffset = float(xpos) - lastX;
    float yOffset = lastY - float(ypos);
    lastX = float(xpos);
    lastY = float(ypos);
    xOffset *= camera.sensitivity;
    yOffset *= camera.sensitivity;
    camera.yaw += xOffset;
    camera.pitch += yOffset;
    if (camera.pitch > 89.0f) {
        camera.pitch = 89.0f;
    }
    if (camera.pitch < -89.0f) {
        camera.pitch = -89.0f;
    }
    camera.forward.x = cos(glm::radians(camera.pitch)) * cos(glm::radians(camera.yaw));
    camera.forward.y = sin(glm::radians(camera.pitch));
    camera.forward.z = cos(glm::radians(camera.pitch)) * sin(glm::radians(camera.yaw));
    camera.forward = glm::normalize(camera.forward);
}
