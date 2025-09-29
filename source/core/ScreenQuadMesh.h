#pragma once
#include <QOpenGLFunctions_4_1_Core>

class ScreenQuadMesh : protected QOpenGLFunctions_4_1_Core{
public: 
    ScreenQuadMesh();
    ~ScreenQuadMesh();

    void draw();

private:
    GLuint vao, vbo;
};