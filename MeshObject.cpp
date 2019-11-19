//
// Created by gerw on 11/13/19.
//

#include <glad/glad.h>
#include <OpenMesh/Core/IO/Options.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "MeshObject.h"
#include "Config.h"


MeshObject::MeshObject(const std::string &name) : name(name) {

    std::string meshPath = Config::assetRoot + "/objects/" + name;
    {
        //come on, read mesh
        OpenMesh::IO::Options opt;
        opt.set(OpenMesh::IO::Options::VertexNormal | OpenMesh::IO::Options::VertexTexCoord);
        if (!OpenMesh::IO::read_mesh(mesh, meshPath, opt)) {
            std::cerr << "read mesh error " << meshPath << std::endl;
        }
    }

    vertices.resize(mesh.n_vertices());
    triangles.resize(mesh.n_faces() * 3);

    for (auto i = 0; i < int(mesh.n_vertices()); i++) {
        vertices[i].position = glm::make_vec3(mesh.points()[i].data());
        vertices[i].texCoords = glm::make_vec2(mesh.texcoords2D()[i].data());
        vertices[i].normal = glm::make_vec3(mesh.vertex_normals()[i].data());
        vertices[i].tangent = glm::vec3(0.0f);
        vertices[i].biTangent = glm::vec3(0.0f);
        for (int j = 0; j < 3; j++) {
            axisMax[j] = std::max(axisMax[j], vertices[i].position[j]);
            axisMin[j] = std::min(axisMin[j], vertices[i].position[j]); // prevbug: axisMin axisMax
        }
    }

    {
        int curIdx = 0;
        for (auto fit = mesh.faces_begin(); fit != mesh.faces_end(); ++fit) {
            std::vector<Vertex> triVerts;
            triVerts.reserve(3);
            for (auto vit = mesh.fv_begin(*fit); vit != mesh.fv_end(*fit); ++vit) {
                triVerts.push_back(vertices[vit->idx()]);
            }
            glm::vec3 edge1 = triVerts[1].position - triVerts[0].position;
            glm::vec3 edge2 = triVerts[2].position - triVerts[0].position;
            glm::vec2 deltaUV1 = triVerts[1].texCoords - triVerts[0].texCoords;
            glm::vec2 deltaUV2 = triVerts[2].texCoords - triVerts[0].texCoords;
            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
            glm::vec3 tangent, biTangent;

            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = glm::normalize(tangent);

            biTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            biTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            biTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
            biTangent = glm::normalize(biTangent);

            for (int i = 0; i < 3; i++) {
                triVerts[i].tangent = tangent;
                triVerts[i].biTangent = biTangent;
                triangles[curIdx++] = triVerts[i];
            }
        }
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, triangles.size() * sizeof(Vertex), triangles.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, tangent));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, biTangent));

    glBindVertexArray(0);
}

void MeshObject::draw() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, triangles.size());
    glBindVertexArray(0);
}

void MeshObject::buildAABBTree() {
    std::vector<int> triIndexes(mesh.n_faces());
    for (int i = 0; i < int(triangles.size()); i += 3) {
        triIndexes.push_back(i);
    }
    root = getAABBTree(triIndexes, 0, triIndexes.size(), 0);
}

MeshObject::~MeshObject() {
    freeTree(root);
}

AABBNode *MeshObject::getAABBTree(std::vector<int> &index, int l, int r, int ax) {
    if (l == r) {
        return nullptr;
    }
    auto me = new AABBNode;
    if (l + 1 == r) {
        me->triIdx = index[l];
        for (int idx = me->triIdx; idx < me->triIdx + 3; idx++) {
            for (int d = 0; d < 3; d++) {
                me->axisMin[d] = std::min(triangles[idx].position[d], me->axisMin[d]);
                me->axisMax[d] = std::max(triangles[idx].position[d], me->axisMax[d]);
            }
        }
    } else {
        std::sort(index.begin() + l, index.begin() + r, [ax, this](int a, int b) {
            return triangles[a].position[ax] < triangles[b].position[ax];
        });
        int mid = (l + r) >> 1;
        me->ch[0] = getAABBTree(index, l, mid, (ax + 1) % 3);
        me->ch[1] = getAABBTree(index, mid, r, (ax + 1) % 3);
        for (auto &c : me->ch) {
            if (c != nullptr) {
                for (int d = 0; d < 3; d++) {
                    me->axisMin[d] = std::min(me->axisMin[d], c->axisMin[d]);
                    me->axisMax[d] = std::max(me->axisMax[d], c->axisMax[d]); // prevbug: axisMin axisMax
                }
            }
        }
    }
    return me;
}

void MeshObject::freeTree(AABBNode *me) {
    if (me == nullptr) {
        return;
    }
    freeTree(me->ch[0]);
    freeTree(me->ch[1]);
    delete me;
}

void MeshObject::publicQueryAABBTree(const Ray &ray, float &nearest, int &triIdx, float &targetU, float &targetV) {
    queryAABBTree(root, ray, nearest, triIdx, targetU, targetV);
}

void
MeshObject::queryAABBTree(AABBNode *me, const Ray &ray, float &nearest, int &triIdx, float &targetU, float &targetV) {
    if (me == nullptr) {
        return;
    }
    if (me->triIdx != -1) {
        float t, u, v;
        if (ray.intersectionTriangle(triangles[me->triIdx].position,
                                     triangles[me->triIdx + 1].position,
                                     triangles[me->triIdx + 2].position,
                                     t, u, v)) {
            if (nearest > t) {
                nearest = t;
                triIdx = me->triIdx;
                targetU = u;
                targetV = v;
            }
        }
    } else {
        float t[2];
        bool ret[2];
        for (int c = 0; c < 2; c++) {
            if (me->ch[c] != nullptr) {
                ret[c] = ray.intersectionAABB(me->ch[c]->axisMin, me->ch[c]->axisMax, t[c]);
            } else {
                ret[c] = false;
            }
        }
        int winner;
        if (ret[0] && ret[1]) {
            winner = t[0] < t[1] ? 0 : 1;
        } else {
            winner = 0;
        }
        for (int i = 0; i < 2; i++) {
            if (ret[winner] && t[winner] < nearest) { // prevbug: ret[winner]
                queryAABBTree(me->ch[winner], ray, nearest, triIdx, targetU, targetV);
            }
            winner ^= 1;
        }
    }
}
