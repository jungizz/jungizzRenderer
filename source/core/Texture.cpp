#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include <stb_image.h>
#include <iostream>

Texture::Texture()
    : id(0), ownsHandle(true){
    initializeOpenGLFunctions();
}

Texture::Texture(GLuint existingHandle, bool owns)
    : id(existingHandle), ownsHandle(owns){
    initializeOpenGLFunctions();
}

Texture::~Texture() {
    if (ownsHandle && id) glDeleteTextures(1, &id);
}

bool Texture::loadFromFile(const std::string& path) {
    int n;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &n, 4);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << "\n";
        return false;
    }

    create(data, width, height);
    stbi_image_free(data);
    return true;
}

void Texture::create(unsigned char* data, GLsizei imgWidth, GLsizei imgHeight){
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, id);
}
