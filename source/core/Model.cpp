#include "Model.h"
#include <iostream>

Model::Model(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FixInfacingNormals
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene) {

    // 임시 데이터
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::u32vec3> indices;

    // 메모리 공간 확보 (삽입 시 재할당 하지 않도록해서 성능 향상)
    vertices.reserve(mesh->mNumVertices);
    normals.reserve(mesh->mNumVertices);
    texcoords.reserve(mesh->mNumVertices);

    // mesh's vertex
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertices.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        if (mesh->HasNormals())
            normals.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        else
            normals.emplace_back(0.0f);

        if (mesh->mTextureCoords[0])
            texcoords.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else
            texcoords.emplace_back(0.0f, 0.0f);
    }

    // mesh's face (triangle)
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        indices.emplace_back(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
    }

    return new Mesh(vertices, normals, texcoords, indices);
}

void Model::draw() const {
    for (auto m : meshes) m->draw();
}
