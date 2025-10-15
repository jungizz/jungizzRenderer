#pragma once
#include "Scene.h"
#include "core/Framebuffer.h"
#include "core/ScreenQuadMesh.h"

class SSSSS_Scene : public Scene{
public:
    SSSSS_Scene();
    ~SSSSS_Scene();

    void init(const glm::ivec2& framebufferSize, QOpenGLFunctions_4_1_Core* f) override;
    void render(const glm::ivec2& framebufferSize) override;
    void resizeFBO(const glm::ivec2& framebufferSize) override;

private:
    Shader *diffShader, *specShader, *rowBlurShader, *colBlurShader;
    
    ScreenQuadMesh quad;

    FBO diffFBO, specFBO, rowBlurFBO, colBlurFBO; // Frame Buffer Objects
    Texture diffFBOColorTex, rowBlurFBOColorTex, colBlurFBOColorTex;
    Texture diffFBODepthTex;
};