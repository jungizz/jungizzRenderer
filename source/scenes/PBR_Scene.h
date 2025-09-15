#pragma once
#include "Scene.h"
#include "core/Shader.h"

class PBR_Scene : public Scene{
public:
    PBR_Scene();
    ~PBR_Scene();

    void init() override;
    void render(const glm::ivec2& framebufferSize) override;

private:
    Shader* shader;
};