#include "Mesh.h"

Mesh::Mesh(const std::vector<glm::vec3>& vertices,
            const std::vector<glm::vec3>& normals,
            const std::vector<glm::vec2>& texcoords,
            const std::vector<glm::u32vec3>& indices)
{
    initializeOpenGLFunctions(); // QOpenGLFunctions_4_1_Core 초기화
    
    indexCount = static_cast<GLsizei>(indices.size()*3);

    // <버텍스 정보 저장>
    // 1. Vertex Array Object(VAO)
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // 2. Vertex Buffer Object(VBO) 
    glGenBuffers(1, &vboVertices); // 버퍼 1개 생성
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices); // 사용할 버퍼 바인딩
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW); // 버퍼에 버텍스 데이터 저장
    glEnableVertexAttribArray(0); // 버텍스어레이 0번 공간 활성화
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); // 특정 속성을 활성화된 공간에 저장 (여긴 vertexPos 속성, 버텍스 당 숫자 3개 x, y, z)

    glGenBuffers(1, &vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &vboTexcoords);
    glBindBuffer(GL_ARRAY_BUFFER, vboTexcoords);
    glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2), texcoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Element Buffer Object(EBO); Index buffer
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::u32vec3), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vboVertices) glDeleteBuffers(1, &vboVertices);
    if (vboNormals) glDeleteBuffers(1, &vboNormals);
    if (vboTexcoords) glDeleteBuffers(1, &vboTexcoords);
    if (ebo) glDeleteBuffers(1, &ebo);
}

void Mesh::draw() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
