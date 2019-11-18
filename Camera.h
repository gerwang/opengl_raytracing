//
// Created by gerw on 11/18/19.
//

#ifndef OPENGL_RAYTRACING_CAMERA_H
#define OPENGL_RAYTRACING_CAMERA_H

#include <glm/glm.hpp>
#include "Config.h"

class Camera {
public:
    float radius{};
    float speed = 0.02f;

    // perspective
    float fovy = glm::radians(45.0f); // prevbug: pls convert to radians
    float aspect = float(Config::windowWidth) / Config::windowHeight;
    float zNear = 0.001f;
    float zFar = 10.0f;

    // orthogonal
    float left = -0.8f;
    float right = 0.8f;
    float bottom = -0.45f;
    float top = 0.45f;

    // view
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 forward;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    float yaw = -90, pitch = 0;

    bool usePerspective = true;

    float sensitivity = 0.05f;

    glm::mat4 getProjectMat();

    glm::mat4 getViewMat();
};


#endif //OPENGL_RAYTRACING_CAMERA_H
