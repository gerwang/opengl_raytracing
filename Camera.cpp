//
// Created by gerw on 11/18/19.
//

#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

glm::mat4 Camera::getProjectMat() {
    if (usePerspective) {
        return glm::perspective(fovy, aspect, zNear, zFar);
    } else {
        return glm::ortho(left, right, bottom, top, zNear, zFar);
    }
}

glm::mat4 Camera::getViewMat() {
    return glm::lookAt(pos, pos + forward, up);
}
