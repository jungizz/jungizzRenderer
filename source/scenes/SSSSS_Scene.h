#pragma once
#include "Scene.h"
#include "core/Shader.h"
#include "core/Framebuffer.h"

class SSSSS_Scene : public Scene{
public:
    SSSSS_Scene();
    ~SSSSS_Scene();

    void init() override;
    void render(const glm::ivec2& framebufferSize) override;

private:
    Shader* shader;
    
    // GLuint quadVertexBuffer, quadArrrayBuffer; // quad vertex data

    // float quadVertices[] = { // 화면 전체에 렌더링하기 위한 사각형 정점 데이터
    //     -1.0f,  1.0f, 0.0f,// 왼쪽 상단
    //     -1.0f, -1.0f, 0.0f,// 왼쪽 하단
    //     1.0f, -1.0f, 0.0f,// 오른쪽 하단

    //     -1.0f,  1.0f, 0.0f,// 왼쪽 상단
    //     1.0f, -1.0f, 0.0f,// 오른쪽 하단
    //     1.0f,  1.0f, 0.0f,// 오른쪽 상단
    // };

    FBO diffFBO, specFBO, rowGaussianFBO, colGaussianFBO; // Frame Buffer Objects
};