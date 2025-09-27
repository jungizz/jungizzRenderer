#pragma once
#include <QOpenGLFunctions>

class ScreenQuadMesh{
public: 
    ScreenQuadMesh();
    ~ScreenQuadMesh();

    void draw();

private:
    GLuint vao, vbo;
};