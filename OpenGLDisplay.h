//
// Created by gerw on 11/13/19.
//

#ifndef OPENGL_RAYTRACING_OPENGLDISPLAY_H
#define OPENGL_RAYTRACING_OPENGLDISPLAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Scene.h"
#include "AssetManager.h"
#include "Camera.h"
#include "shader.h"

class OpenGLDisplay {
public:

    OpenGLDisplay();

    ~OpenGLDisplay();

    static void initGL();


    void mainLoop();

    void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos);

    static bool glInited;

private:

    void imGuiStart();

    void imGuiEnd();

    void processKeyInput();

    GLFWwindow *window;

    std::unique_ptr<Scene> scene;
    AssetManager assetManager;
    Camera camera;

    std::unique_ptr<Shader> shader, shadowShader;

    // input status
    bool useRayTracing = false;
    int rayTracingIter = 0;

    bool showUI = true;
    float lastX, lastY;
    bool firstMouse = true;

    // shadow map
    int shadowWidth = 1024, shadowHeight = 1024;
    GLuint depthMap, depthMapFBO;
};


#endif //OPENGL_RAYTRACING_OPENGLDISPLAY_H
