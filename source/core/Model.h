#pragma once
#include "Mesh.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

class Model {
public:
    Model(const std::string& path);
    void draw() const;

private:
    std::vector<Mesh*> meshes;

    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
};