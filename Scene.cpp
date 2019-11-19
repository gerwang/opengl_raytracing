//
// Created by gerw on 11/18/19.
//

#include <fstream>
#include <iostream>
#include "Scene.h"
#include "Ray.h"

Scene::Scene(const std::string &jsonPath) {
    Json::CharReaderBuilder rBuilder;
    rBuilder["collectComments"] = false;
    std::string errs;
    std::ifstream fin(jsonPath);
    bool ok = Json::parseFromStream(rBuilder, fin, &root, &errs);
    if (!ok) {
        std::cerr << errs << std::endl;
        return;
    }
    loaded = true;

    for (auto light:root["lights"]) {
        LightRef ref{};
        for (int i = 0; i < 3; i++) {
            ref.pos[i] = light["pos"][i].asFloat();
            ref.color[i] = light["color"][i].asFloat();
        }
        lights.push_back(ref);
    }
}

void Scene::loadAssets(AssetManager &manager) {
    assetManager = &manager;
    Json::Value objects = root["objects"];
    for (auto obj : objects) {
        std::string ply = obj["ply"].asString();
        manager.loadObject(ply);
        std::string texture = obj["texture"].asString();
        manager.loadTexture(texture, AssetManager::COLOR);
        std::string normal = obj["normal"].asString();
        manager.loadTexture(normal, AssetManager::NORMAL);
        MeshRef ref;
        ref.ply = ply;
        ref.texture = texture;
        ref.normal = normal;
        ref.collisionAABB = obj["collision"].asString() == "aabb";
        ref.isLight = obj["light"].asBool();
        for (int i = 0; i < 3; i++) {
            ref.reflectance[i] = obj["reflect"][i].asFloat();
            ref.refractance[i] = obj["refract"][i].asFloat();
        }
        ref.reflectCone = glm::radians(obj["cone"].asFloat());
        ref.refractEta = obj["eta"].asFloat();
        meshRefs.push_back(ref);
    }
}

void Scene::initCamera(Camera &camera) {
    Json::Value cameraJson = root["camera"];
    for (int i = 0; i < 3; i++) {
        camera.pos[i] = cameraJson["pos"][i].asFloat();
    }
    camera.pitch = cameraJson["pitch"].asFloat();
    camera.yaw = cameraJson["yaw"].asFloat();
    camera.radius = cameraJson["radius"].asFloat();
}

glm::vec3 Scene::clipMove(const Camera &camera, const glm::vec3 &pos, glm::vec3 delta) {
    // model mat must be identity
    if (delta == glm::vec3(0.0f)) {
        return delta;
    }
    float len = glm::length(delta);
    delta /= len;
    Ray ray{pos, delta};
    float tMin = 1e9f;
    for (auto &mesh:meshRefs) {
        auto &obj = assetManager->meshMap[mesh.ply];
        if (mesh.collisionAABB) {
            float t;
            if (ray.intersectionAABB(obj->axisMin, obj->axisMax, t)) {
                tMin = std::min(tMin, t);
            }
        } else {
            float t, u, v;
            for (int i = 0; i < int(obj->triangles.size()); i += 3) {
                if (ray.intersectionTriangle(obj->triangles[i].position, obj->triangles[i + 1].position,
                                             obj->triangles[i + 2].position, t, u, v)) {
                    tMin = std::min(tMin, t);
                }
            }
        }
    }
    tMin = std::min(tMin - camera.radius, len);
    tMin = std::max(tMin, 0.0f);
    return delta * tMin;
}
