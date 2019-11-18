//
// Created by gerw on 11/13/19.
//

#ifndef OPENGL_RAYTRACING_MESHOBJECT_H
#define OPENGL_RAYTRACING_MESHOBJECT_H

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <glm/glm.hpp>

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


class MeshObject {
public:
    explicit MeshObject(const std::string &name);

    MyMesh mesh;
    std::string name;
    std::vector<Vertex> vertices, triangles;

    unsigned vao{}, vbo{};

    glm::vec3 axisMin{}, axisMax{};

    void draw();
};


#endif //OPENGL_RAYTRACING_MESHOBJECT_H
