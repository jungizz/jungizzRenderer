#pragma once
#include <glm/glm.hpp>
#include "core/Camera.h"

class Scene{
public:
    virtual ~Scene() {}
    virtual void init() = 0;
    virtual void render(const glm::ivec2& framebufferSize) = 0;
    virtual Camera* getCamera() = 0;
};