#pragma once
#include <GL/glew.h>
#include <string>

class Texture{
public:
    Texture();
    ~Texture();

    bool loadFromFile(const std::string& path);
    void bind(GLenum textureUnit = GL_TEXTURE0) const;
    GLuint getID() const { return id; }
    
private:
    GLuint id;
};