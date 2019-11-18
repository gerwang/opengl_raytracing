//
// Created by gerw on 11/18/19.
//

#include <fstream>
#include <iostream>
#include "Scene.h"

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
