#pragma once
#include <glm/glm.hpp>
#include "core/Model.h"
#include "core/Texture.h"
#include "core/Camera.h"
#include "core/Light.h"
#include "core/Shader.h"

class Scene{
public:
    virtual ~Scene();
    virtual void init(const glm::ivec2& framebufferSize) = 0;
    virtual void render(const glm::ivec2& framebufferSize) = 0;
    virtual void resizeFBO(const glm::ivec2& framebufferSize){ } // fbo를 사용하는 자식 씬만 오버라이드 해서 쓰면 됨
    void beginPass(const glm::ivec2& size, bool useDepth);

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