#include "ScreenQuadMesh.h"

ScreenQuadMesh::ScreenQuadMesh(){
    initializeOpenGLFunctions(); // QOpenGLFunctions 초기화
    
    float quadVertices[] = { // 화면 전체에 렌더링하기 위한 사각형 정점 데이터 (삼각형 2개)
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,  // 왼쪽 상단
        -1.0f, -1.0f,  0.0f, 0.0f,  // 왼쪽 하단
         1.0f, -1.0f,  1.0f, 0.0f,  // 오른쪽 하단

        -1.0f,  1.0f,  0.0f, 1.0f,  // 왼쪽 상단
         1.0f, -1.0f,  1.0f, 0.0f,  // 오른쪽 하단
         1.0f,  1.0f,  1.0f, 1.0f   // 오른쪽 상단
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // layout(location = 0) vec2 position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // layout(location = 1) vec2 texCoords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
}

ScreenQuadMesh::~ScreenQuadMesh(){
    if(vao) glDeleteBuffers(1, &vao);
    if(vbo) glDeleteBuffers(1, &vbo);
}

void ScreenQuadMesh::draw(){
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}