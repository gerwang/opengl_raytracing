//
// Created by gerw on 11/18/19.
//

#ifndef OPENGL_RAYTRACING_SCENE_H
#define OPENGL_RAYTRACING_SCENE_H

#include <string>
#include <json/json.h>
#include "AssetManager.h"

class MeshRef {
public:
    std::string ply, texture, normal;
    glm::mat4 modelMat;
};

class Scene {
public:
    explicit Scene(const std::string &jsonPath);

    void loadAssets(AssetManager &manager);

    std::vector<MeshRef> meshRefs;

private:
    Json::Value root;
    bool loaded = false;
};


#endif //OPENGL_RAYTRACING_SCENE_H
