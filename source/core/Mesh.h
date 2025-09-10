#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Mesh {
public:
    Mesh(const std::vector<glm::vec3>& vertices,
         const std::vector<glm::vec3>& normals,
         const std::vector<glm::vec2>& texcoords,
         const std::vector<glm::u32vec3>& indices);
    ~Mesh();

    void draw() const;

private:
    GLuint vao, vboVertices, vboNormals, vboTexcoords, ebo; // buffer ID
    GLsizei indexCount;
};
