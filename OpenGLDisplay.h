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


    static bool glInited;

private:

    void imGuiStart();

    void imGuiEnd();

    GLFWwindow *window;

    std::unique_ptr<Scene> scene;
    AssetManager assetManager;
    Camera camera;

    std::unique_ptr<Shader> shader;

    // input status
    bool useRayTracing = false;
};


#endif //OPENGL_RAYTRACING_OPENGLDISPLAY_H
