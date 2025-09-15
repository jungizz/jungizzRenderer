#pragma once
#include <glm/glm.hpp>

class Camera{
public:
    Camera(float fovy = glm::radians(30.0f), float distance = 2.0f, float minDistance = 0.01f, float maxDistance = 10.0f);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspect) const;
    glm::vec3 getPosition() const;

    void setTarget(const glm::vec3& t) { target = t; }

    void mousePressProcess(int button, int action, double xpos, double ypos);
    void mouseMoveProcess(double xpos, double ypos);
    void scrollProcess(double delta);

private:
    glm::vec2 lastCursorPos;
    glm::vec3 target; // 카메라가 바라보는 중심점 (패닝 시 이동)

    bool rotating = false;
    bool panning = false;

    float theta, phi; // 수평/수직 회전 각도
    float fovy; // 시야각 (30*PI/100)
    float distance;
    float zoomSpeed;
    float panSpeed;
    float minDistance, maxDistance;
};