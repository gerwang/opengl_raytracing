//
// Created by gerw on 11/18/19.
//

#ifndef OPENGL_RAYTRACING_SCENE_H
#define OPENGL_RAYTRACING_SCENE_H

#include <string>
#include <json/json.h>
#include "AssetManager.h"
#include "Camera.h"
#include "shader.h"

class MeshRef {
public:
    std::string ply, texture, normal;
    glm::mat4 modelMat = glm::mat4(1.0f);
    bool collisionAABB;

    glm::vec3 reflectance, refractance;
    bool isLight;
    float mirrorRatio;
    float refractEta;
    bool blendNormal;
};

class LightRef {
public:
    glm::vec3 pos;
    glm::vec3 color;
};

class Scene {
public:
    explicit Scene(const std::string &jsonPath);

    void loadAssets(AssetManager &manager);

    void initCamera(Camera &camera);

    glm::vec3 clipMove(const Camera &camera, const glm::vec3 &pos, glm::vec3 delta);

    std::vector<MeshRef> meshRefs;

    std::vector<LightRef> lights;


private:
    AssetManager *assetManager{};
    Json::Value root;
    bool loaded = false;
};


#endif //OPENGL_RAYTRACING_SCENE_H
