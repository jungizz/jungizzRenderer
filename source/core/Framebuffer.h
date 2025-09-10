#pragma once
#include <GL/glew.h>

struct FBO{
    GLuint framebuffer = 0;
    GLuint colorTexBuffer = 0;
    GLuint depthBuffer = 0;
};