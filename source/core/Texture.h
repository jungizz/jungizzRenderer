#pragma once
#include <QOpenGLFunctions>
#include <string>

class Texture : protected QOpenGLFunctions{
public:
    Texture();
    ~Texture();

    bool loadFromFile(const std::string& path);
    void bind(GLenum textureUnit = GL_TEXTURE0);
    GLuint getID() const { return id; }
    
private:
    GLuint id;
};