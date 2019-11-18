//
// Created by gerw on 11/18/19.
//

#ifndef OPENGL_RAYTRACING_CAMERA_H
#define OPENGL_RAYTRACING_CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    float radius{};

    // perspective
    float fovy = 25.0f;
    float aspect = 9.0f / 16.0f;
    float zNear = 0.1f;
    float zFar = 100.0f;

    // orthogonal
    float left = -1.0f;
    float right = 1.0f;
    float bottom = -1.0f;
    float top = 1.0f;

    bool usePerspective = true;

    glm::mat4 getProjectMat();

    glm::mat4 getViewMat();
};


#endif //OPENGL_RAYTRACING_CAMERA_H
