//
// Created by gerw on 11/13/19.
//

#ifndef OPENGL_RAYTRACING_OPENGLDISPLAY_H
#define OPENGL_RAYTRACING_OPENGLDISPLAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <random>
#include "Scene.h"
#include "AssetManager.h"
#include "Camera.h"
#include "shader.h"
#include "Ray.h"

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

    glm::vec3 rayTracing(const Ray &ray, int depth, const glm::vec3 &prevIntensity, std::mt19937 &gen,
                         std::uniform_real_distribution<> &dis);

    GLFWwindow *window;

    std::unique_ptr<Scene> scene;
    AssetManager assetManager;
    Camera camera;

    std::unique_ptr<Shader> shader, shadowShader, shaderQuad;

    // input status
    bool useRayTracing = false;
    int rayTracingIter = 0;
    std::vector<glm::vec3> renderBuffer;

    bool showUI = true;
    float lastX, lastY;
    bool firstMouse = true;

    // shadow map
    int shadowWidth = 1024, shadowHeight = 1024;
    GLuint depthMap, depthMapFBO;
    GLuint rayTexture;
    GLuint quadVAO{}, quadVBO{};
};


#endif //OPENGL_RAYTRACING_OPENGLDISPLAY_H
