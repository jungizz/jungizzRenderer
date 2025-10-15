#pragma once
#include "Scene.h"

class PBR_Scene : public Scene{
public:
    PBR_Scene();
    ~PBR_Scene();

    void init(const glm::ivec2& framebufferSize, QOpenGLFunctions_4_1_Core* f) override;
    void render(const glm::ivec2& framebufferSize) override;

private:
    Shader* shader;
};