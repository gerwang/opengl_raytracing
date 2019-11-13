//
// Created by gerw on 11/13/19.
//

#ifndef OPENGL_RAYTRACING_OPENGLDISPLAY_H
#define OPENGL_RAYTRACING_OPENGLDISPLAY_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>


class OpenGLDisplay {
public:

    OpenGLDisplay();

    static void initGL();


    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }

    static bool glInited;

private:
    GLFWwindow *window;
};


#endif //OPENGL_RAYTRACING_OPENGLDISPLAY_H
