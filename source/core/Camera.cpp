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

void Camera::cursorPosCallback(GLFWwindow* win, double xpos, double ypos){
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(win));
    double dx = xpos - cam->lastCursorPos.x;
    double dy = ypos - cam->lastCursorPos.y;

    int w, h;
    glfwGetWindowSize(win, &w, &h);

    // 좌클릭 -> 회전
    if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        cam->theta -= dx / w * glm::pi<float>(); // 마우스 x축 이동량에 PI 곱하여 라디안 단위로 변환
        cam->phi += dy / h * glm::pi<float>();   // 마우스 y축 이동량에 PI 곱하여 라디안 단위로 변환
    }

    // 휠 버튼 -> 패닝
    if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        float panX = -dx * cam->panSpeed * cam->distance;
        float panY = dy * cam->panSpeed * cam->distance;

        // 카메라 right, up 벡터 구하기
        glm::vec3 eye = cam->getPosition();
        glm::vec3 forward = glm::normalize(cam->target - eye);
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));
        glm::vec3 up = glm::normalize(glm::cross(right, forward));

        // 카메라가 바라보는 중심점 이동
        cam->target += right * panX + up * panY;
    }    

    cam->lastCursorPos.x = xpos;
    cam->lastCursorPos.y = ypos;
}

void Camera::scrollCallback(GLFWwindow* win, double xoffset, double yoffset){
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(win));

    cam->distance -= static_cast<float>(yoffset) * cam->zoomSpeed; // yoffset: 휠을 위로 올릴 때 양수, 아래로 내릴 때 음수
    if (cam->distance < cam->minDistance) cam->distance = cam->minDistance;
    if (cam->distance > cam->maxDistance) cam->distance = cam->maxDistance;
}

