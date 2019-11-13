//
// Created by gerw on 11/13/19.
//

#ifndef OPENGL_RAYTRACING_OPENGLDISPLAY_H
#define OPENGL_RAYTRACING_OPENGLDISPLAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
};


#endif //OPENGL_RAYTRACING_OPENGLDISPLAY_H
