#pragma once
#include <glm/glm.hpp>

struct Light{
    glm::vec3 position;
    glm::vec3 color;
    float intensity;

    Light(const glm::vec3& pos = glm::vec3(0), const glm::vec3& col = glm::vec3(1), float i = 1.0f)
        : position(pos), color(col), intensity(i) {}
};