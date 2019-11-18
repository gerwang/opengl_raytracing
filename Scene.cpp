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
        // TODO model mat
        meshRefs.push_back(ref);
    }
}
