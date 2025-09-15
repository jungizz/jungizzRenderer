#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fovy, float distance, float minDistance, float maxDistance)
    :theta(0.0f), phi(0.0f), fovy(fovy), distance(distance),
     zoomSpeed(0.05f), panSpeed(0.002f), 
     minDistance(minDistance), maxDistance(maxDistance),
     lastCursorPos(0.0f), target(0.0f) {}

glm::mat4 Camera::getViewMatrix() const{
    return glm::lookAt(getPosition(), target, 
                        (cos(phi) > 0) ? glm::vec3(0,1,0) : glm::vec3(0,-1,0)); 
                        // phi가 ±90도를 넘으면 회전 축이 반전돼서 상하반전 되므로 이를 방지하기 위해 (0, -1, 0) 설정
}

glm::mat4 Camera::getProjectionMatrix(float aspect) const{
    return glm::perspective(fovy, aspect, minDistance, maxDistance);
}

glm::vec3 Camera::getPosition() const{
    float x = distance * cos(phi) * sin(theta);
    float y = distance * sin(phi);
    float z = distance * cos(phi) * cos(theta);
    return target + glm::vec3(x, y, z);
}

void Camera::mousePressProcess(int button, int action, double xpos, double ypos){
    lastCursorPos = glm::vec2(xpos, ypos);

    if(button == 0) rotating = (action == 1); // Left button
    else if(button == 1) panning = (action == 1); // Right button
}

void Camera::mouseMoveProcess(double xpos, double ypos){
    double dx = xpos - lastCursorPos.x;
    double dy = ypos - lastCursorPos.y;

    // Left button -> rotating
    if (rotating) {
        theta -= dx * 0.01f;
        phi += dy * 0.01f;
    }

    // Right button -> panning
    if (panning) {
        float panX = -dx * panSpeed * distance;
        float panY = dy * panSpeed * distance;

        // 카메라 right, up 벡터 구하기
        glm::vec3 eye = getPosition();
        glm::vec3 forward = glm::normalize(target - eye);
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));
        glm::vec3 up = glm::normalize(glm::cross(right, forward));

        // 카메라가 바라보는 중심점 이동
        target += right * panX + up * panY;
    }    

    lastCursorPos = glm::vec2(xpos, ypos);
}

void Camera::scrollProcess(double delta){
    distance -= static_cast<float>(delta) * zoomSpeed; // yoffset: 휠을 위로 올릴 때 양수, 아래로 내릴 때 음수
    if (distance < minDistance) distance = minDistance;
    if (distance > maxDistance) distance = maxDistance;
}

