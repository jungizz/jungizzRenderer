#pragma once
#include <QOpenGLFunctions>

struct FBO : protected QOpenGLFunctions {
    GLuint framebuffer = 0;
    GLuint colorTexBuffer = 0;
    GLuint depthBuffer = 0;

    void init() {
        initializeOpenGLFunctions();
        glGenFramebuffers(1, &framebuffer);
        glGenTextures(1, &colorTexBuffer);
        glGenRenderbuffers(1, &depthBuffer);
    }

    void cleanup() {
        glDeleteFramebuffers(1, &framebuffer);
        glDeleteTextures(1, &colorTexBuffer);
        glDeleteRenderbuffers(1, &depthBuffer);
    }
};
