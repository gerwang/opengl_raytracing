//
// Created by gerw on 11/18/19.
//

#include "Ray.h"
#include <algorithm>

const float eps = 1e-9;

bool Ray::intersectionAABB(const glm::vec3 &axisMin, const glm::vec3 &axisMax, float &t) const {
    float tMin = 0, tMax = 1e20;
    for (int i = 0; i < 3; i++) {
        if (fabs(direction[i]) < eps) {
            if (axisMin[i] <= pos[i] && pos[i] <= axisMax[i]) {
                continue;
            } else {
                return false;
            }
        } else {
            float t1 = (axisMax[i] - pos[i]) / direction[i];
            float t2 = (axisMin[i] - pos[i]) / direction[i];
            tMin = std::max(tMin, t2);
            tMax = std::min(tMax, t1);
            if (tMin > tMax) {
                return false;
            }
        }
    }
    t = tMin;
    return true;
}

bool Ray::intersectionTriangle(const glm::vec3 &pos0, const glm::vec3 &pos1, const glm::vec3 &pos2, float &t, float &u,
                               float &v) const {
    auto E1 = pos1 - pos0;
    auto E2 = pos2 - pos0;
    auto P = glm::cross(direction, E2);
    float det = glm::dot(E1, P);
    glm::vec3 T;
    if (det > 0) {
        T = pos - pos0;
    } else {
        T = pos0 - pos;
        det = -det;
    }
    if (det < eps) {
        return false;
    }
    u = glm::dot(T, P);
    if (u < 0.0f || u > det) {
        return false;
    }

    auto Q = glm::cross(T, E1);
    v = glm::dot(direction, Q);
    if (v < 0.0f || u + v > det) {
        return false;
    }

    t = glm::dot(E2, Q);
    float fINvDet = 1.0f / det;
    t *= fINvDet;
    u *= fINvDet;
    v *= fINvDet;

    return t > eps; // must be positive
}
