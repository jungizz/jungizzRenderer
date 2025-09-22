#pragma once
#include <glm/glm.hpp>
#include "core/Model.h"
#include "core/Texture.h"
#include "core/Camera.h"
#include "core/Light.h"

class Scene{
public:
    virtual ~Scene();
    virtual void init() = 0;
    virtual void render(const glm::ivec2& framebufferSize) = 0;
    
    void loadModel(const std::string& path);
    void loadTexture(const std::string& type, const std::string& path);
    Light* getLight() { return light; }
    Camera* getCamera() { return camera; }

protected:
    Model* model = nullptr;
    Texture* diffTex = nullptr;
    Texture* normTex = nullptr;
    Texture* roughTex = nullptr;
    Texture* metalTex = nullptr;
    Texture* aoTex = nullptr;
    Camera* camera = nullptr;
    Light* light;

    void setCameraTarget(glm::vec3 target);
};