//
// Created by gerw on 11/13/19.
//

#include "AssetManager.h"
#include "Config.h"

void AssetManager::loadTexture(const std::string &name, int type) {
    std::unique_ptr<Texture> ptr = std::make_unique<Texture>(name);
    textureMap[type][name] = std::move(ptr);
}

void AssetManager::loadObject(const std::string &name) {
    std::unique_ptr<MeshObject> ptr = std::make_unique<MeshObject>(name);
    meshMap[name] = std::move(ptr);
}
