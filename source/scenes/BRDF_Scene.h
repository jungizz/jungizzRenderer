#pragma once
#include "Scene.h"
#include "core/Shader.h"
#include "core/Texture.h"
#include "core/Camera.h"
#include "core/Model.h"
#include "core/Light.h"

class BRDF_Scene : public Scene{
public:
    BRDF_Scene();
    ~BRDF_Scene();

    void init() override;
    void render(const glm::ivec2& framebufferSize) override;

    Camera* getCamera() override { return camera; }

private:
    Shader* shader;
    Camera* camera;
    Model* model;

    Texture* diffTex;
    Texture* normTex;
    Texture* roughTex;
    Texture* metalTex;
    Texture* aoTex;

    Light light;
    
};