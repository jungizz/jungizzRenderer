#pragma once
#include <QOpenGLFunctions>

class ScreenQuadMesh : protected QOpenGLFunctions{
public: 
    ScreenQuadMesh();
    ~ScreenQuadMesh();

    void draw();

private:
    GLuint vao, vbo;
};