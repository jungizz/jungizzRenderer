#include "SSSSS_Scene.h"
#include <iostream>

using namespace glm;

SSSSS_Scene::SSSSS_Scene()
    : diffShader(nullptr), specShader(nullptr), 
      rowBlurShader(nullptr), colBlurShader(nullptr){}

SSSSS_Scene::~SSSSS_Scene() { 
    delete diffShader;
    delete specShader;
    delete rowBlurShader;
    delete colBlurShader;
}

void SSSSS_Scene::init(const glm::ivec2& framebufferSize){
    // 모델 로드
    loadModel("resources/LPS_Head.obj");

    // 셰이더 로드
    diffShader = new Shader("shader/basic_shader.vert", "shader/sssss_diffuse_shader.frag"); // 셰이더 이름 다 바꾸기
    specShader = new Shader("shader/basic_shader.vert", "shader/sssss_specular_shader.frag");
    rowBlurShader = new Shader("shader/quad_shader.vert", "shader/sssss_rowBlur_shader.frag");
    colBlurShader = new Shader("shader/quad_shader.vert", "shader/sssss_colBlur_shader.frag");

    // FBO 세팅 (attach)
    diffFBO.init(framebufferSize.x, framebufferSize.y);
    specFBO.init(framebufferSize.x, framebufferSize.y);
    rowBlurFBO.init(framebufferSize.x, framebufferSize.y);
    colBlurFBO.init(framebufferSize.x, framebufferSize.y);

    // FBO 결과를 받을 텍스처 생성
    diffFBOColorTex.warp(diffFBO.colorTexBuffer, false);
    diffFBODepthTex.warp(diffFBO.depthBuffer, false);
    rowBlurFBOColorTex.warp(rowBlurFBO.colorTexBuffer, false);
    colBlurFBOColorTex.warp(colBlurFBO.colorTexBuffer, false);

    // 카메라 세팅
    setCameraTarget(vec3(0.0f));

    // 라이트 세팅
    light = new Light(vec3(0.0f, 0.0f, 100.0f), vec3(1.0f), 3.0f);
}

void SSSSS_Scene::render(const ivec2& framebufferSize){

    // ======= Pass 1. draw on diffFBO (diffuse BRDF) =======
    diffFBO.bind();
    beginPass(framebufferSize, true);
    diffShader->use();

    diffShader->setMat4("modelMat", mat4(1.0f));
    diffShader->setMat4("viewMat", camera->getViewMatrix());
    diffShader->setMat4("projMat", camera->getProjectionMatrix((float)framebufferSize.x/framebufferSize.y));
    diffShader->setVec3("cameraPosition", camera->getPosition());
    diffShader->setVec3("lightPosition", light->position);
    diffShader->setVec3("lightColor", light->color * light->intensity);
    //shader->setVec3("ambientLight", vec3(0.0));

    normTex->bind(GL_TEXTURE0);
    diffShader->setInt("normTex", 0);

    model->draw();
    diffFBO.unbind();
    
    // ======= Pass 2. draw on rowBlurFBO (row Gaussian Blur) =======
    rowBlurFBO.bind();
    beginPass(framebufferSize, true);
    rowBlurShader->use();

    diffFBOColorTex.bind(GL_TEXTURE0); // diffFBO의 텍스처 사용
    rowBlurShader->setInt("colorTex", 0);

    diffFBODepthTex.bind(GL_TEXTURE1);
    rowBlurShader->setInt("depthTex", 1);

    rowBlurShader->setFloat2("size", (float)framebufferSize.x, (float)framebufferSize.y);
    //rowBlurShader->setFloat("screenWidth", 2 * 0.01 * (tan(camera->getFovy() / 2 * ((float)framebufferSize.x / framebufferSize.y)))); // screen width in world coord (0.01은 near임)
    rowBlurShader->setFloat("fovy", camera->getFovy());
    rowBlurShader->setFloat("near", camera->getNear()); 
    rowBlurShader->setFloat("far", camera->getFar()); 
    rowBlurShader->setFloat("kernelParam", 2.0f); // 기존 코드에서 0.5~3.0 UI 조절 값

    quad.draw();
    rowBlurFBO.unbind();

    // ======= Pass 3. draw on colBlurFBO (col Gaussian Blur) =======
    colBlurFBO.bind();
    beginPass(framebufferSize, true);
    colBlurShader->use();

    rowBlurFBOColorTex.bind(GL_TEXTURE0);
    colBlurShader->setInt("colorTex", 0);

    diffFBODepthTex.bind(GL_TEXTURE1);
    colBlurShader->setInt("depthTex", 1);

    colBlurShader->setFloat2("size", (float)framebufferSize.x, (float)framebufferSize.y);
    colBlurShader->setFloat("screenHeight", 2*0.01*(tan(camera->getFovy()/2))); // screen height in world coord 
    colBlurShader->setFloat("fovy", camera->getFovy());
    colBlurShader->setFloat("near", camera->getNear()); 
    colBlurShader->setFloat("far", camera->getFar()); 
    colBlurShader->setFloat("kernelParam", 1.0f); // 기존 코드에서 0.5~3.0 UI 조절 값

    quad.draw();
    colBlurFBO.unbind();

    // ======= Pass 4. draw on specFBO (specualr BRDF) =======
    beginPass(framebufferSize, true);
    specShader->use();

    specShader->setMat4("modelMat", mat4(1.0f));
    specShader->setMat4("viewMat", camera->getViewMatrix());
    specShader->setMat4("projMat", camera->getProjectionMatrix((float)framebufferSize.x/framebufferSize.y));
    specShader->setVec3("cameraPosition", camera->getPosition());
    specShader->setVec3("lightPosition", light->position);
    specShader->setVec3("lightColor", light->color * light->intensity);
    //shader->setVec3("ambientLight", vec3(0.0));

    diffTex->bind(GL_TEXTURE0);
    specShader->setInt("diffTex", 0);

    roughTex->bind(GL_TEXTURE1);
    specShader->setInt("roughTex", 1);

    colBlurFBOColorTex.bind(GL_TEXTURE2); 
    specShader->setInt("gaussianDiffTex", 2);

    diffFBOColorTex.bind(GL_TEXTURE3);
    specShader->setInt("pass1Tex", 3); // test용

    specShader->setVec3("specReflectance", vec3(0.028)); // 기존 코드에서 0.0~1.0 UI 조절 값
    specShader->setFloat2("size", (float)framebufferSize.x, (float)framebufferSize.y);
    specShader->setInt("selectScene", 0); // 기존 코드에서 SSSSS과정 볼 수 있는 UI 선택 값

    model->draw();
}

void SSSSS_Scene::resizeFBO(const glm::ivec2& framebufferSize){
    diffFBO.resize(framebufferSize.x, framebufferSize.y);
    specFBO.resize(framebufferSize.x, framebufferSize.y);
    rowBlurFBO.resize(framebufferSize.x, framebufferSize.y);
    colBlurFBO.resize(framebufferSize.x, framebufferSize.y);

    diffFBOColorTex.warp(diffFBO.colorTexBuffer, false);
    diffFBODepthTex.warp(diffFBO.depthBuffer, false);
    rowBlurFBOColorTex.warp(rowBlurFBO.colorTexBuffer, false);
    colBlurFBOColorTex.warp(colBlurFBO.colorTexBuffer, false);
}

