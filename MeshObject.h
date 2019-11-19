//
// Created by gerw on 11/13/19.
//

#ifndef OPENGL_RAYTRACING_MESHOBJECT_H
#define OPENGL_RAYTRACING_MESHOBJECT_H

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <glm/glm.hpp>
#include "Ray.h"

const float inf = 1e9;

struct MyTraits : public OpenMesh::DefaultTraits {
    VertexAttributes(OpenMesh::Attributes::Normal | OpenMesh::Attributes::TexCoord2D);
};

typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> MyMesh;

struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoords;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 biTangent;
};

struct AABBNode {
    glm::vec3 axisMin{inf}, axisMax{-inf};
    AABBNode *ch[2]{};
    int triIdx{-1};
};

class MeshObject {
public:
    explicit MeshObject(const std::string &name);

    virtual ~MeshObject();

    MyMesh mesh;
    std::string name;
    std::vector<Vertex> vertices, triangles;

    unsigned vao{}, vbo{};

    glm::vec3 axisMin{inf}, axisMax{-inf};

    AABBNode *root{};

    void freeTree(AABBNode *me);

    void draw();

    void publicQueryAABBTree(const Ray &ray, float &nearest, int &triIdx, float &targetU, float &targetV);

    void buildAABBTree();

    AABBNode *getAABBTree(std::vector<int> &index, int l, int r, int ax);

private:
    void queryAABBTree(AABBNode *me, const Ray &ray, float &nearest, int &triIdx, float &targetU, float &targetV);
};


#endif //OPENGL_RAYTRACING_MESHOBJECT_H
