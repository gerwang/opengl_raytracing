//
// Created by gerw on 11/13/19.
//

#ifndef OPENGL_RAYTRACING_ASSETMANAGER_H
#define OPENGL_RAYTRACING_ASSETMANAGER_H

#include <map>
#include <string>
#include <memory>
#include "Texture.h"
#include "MeshObject.h"

class AssetManager {
public:
    enum TextureType {
        COLOR,
        NORMAL
    };
    std::map<std::string, std::unique_ptr<Texture>> textureMap[2];
    std::map<std::string, std::unique_ptr<MeshObject>> meshMap;

    void loadTexture(const std::string &name, int type);

    void loadObject(const std::string &name);
};


#endif //OPENGL_RAYTRACING_ASSETMANAGER_H
