//
// Created by gerw on 11/18/19.
//

#ifndef OPENGL_RAYTRACING_RAY_H
#define OPENGL_RAYTRACING_RAY_H

#include <glm/glm.hpp>

class Ray {
public:
    glm::vec3 pos;
    glm::vec3 direction;

    bool intersectionAABB(const glm::vec3 &axisMin, const glm::vec3 &axisMax, float &t) const;
    bool intersectionTriangle(const glm::vec3& pos0, const glm::vec3& pos1, const glm::vec3& pos2, float& t, float& u, float& v) const;
};


#endif //OPENGL_RAYTRACING_RAY_H
