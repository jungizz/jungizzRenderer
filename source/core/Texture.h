#pragma once
#include <QOpenGLFunctions>
#include <string>

class Texture : protected QOpenGLFunctions{
public:
    Texture();
    //Texture(GLuint existingHandle, bool ownsHandle = false);
    ~Texture();

    void warp(GLuint existingHandle, bool owns);
    bool loadFromFile(const std::string& path);
    void create(unsigned char* data, GLsizei imgWidth, GLsizei imgHeight);
    void bind(GLenum textureUnit = GL_TEXTURE0);
    GLuint getID() const { return id; }
    
private:
    GLuint id;
    int width, height;
    bool ownsHandle = true;
};