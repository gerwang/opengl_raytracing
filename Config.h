//
// Created by gerw on 11/13/19.
//

#ifndef OPENGL_RAYTRACING_CONFIG_H
#define OPENGL_RAYTRACING_CONFIG_H

#include <string>

class Config {
public:
    const static int windowWidth = 1280;
    const static int windowHeight = 720;
    const static int rayWidth = 128;
    const static int rayHeight = 72;
    const static int rayMaxDepth = 10;
    constexpr const static float rayThreshold = 0.01f;
    const static std::string assetRoot;
};


#endif //OPENGL_RAYTRACING_CONFIG_H
