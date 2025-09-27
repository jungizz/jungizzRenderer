#include "PBR_Scene.h"
#include <iostream>

using namespace glm;

PBR_Scene::PBR_Scene()
    : shader(nullptr){}

PBR_Scene::~PBR_Scene() { 
    delete shader;
}

void PBR_Scene::init(const glm::ivec2& framebufferSize){

    // 모델 로드
    loadModel("resources/spaceHelmet.obj");

    // 셰이더 로드
    shader = new Shader("shader/basic_shader.vert", "shader/pbr_shader.frag");

    // 카메라 세팅
    setCameraTarget(vec3(0.0f));

    // 라이트 세팅
    light = new Light(vec3(0.0f, 0.0f, 100.0f), vec3(1.0f), 3.0f);
}

void PBR_Scene::render(const ivec2& framebufferSize){
    beginPass(framebufferSize, true);

    shader->use();

    shader->setMat4("modelMat", mat4(1.0f));
    shader->setMat4("viewMat", camera->getViewMatrix());
    shader->setMat4("projMat", camera->getProjectionMatrix((float)framebufferSize.x/framebufferSize.y));
    shader->setVec3("cameraPosition", camera->getPosition());

    shader->setVec3("lightPosition", light->position);
    shader->setVec3("lightColor", light->color * light->intensity);
    //shader->setVec3("ambientLight", vec3(0.0));

    diffTex->bind(GL_TEXTURE0);
    shader->setInt("diffTex", 0);

    normTex->bind(GL_TEXTURE1);
    shader->setInt("normTex", 1);

    roughTex->bind(GL_TEXTURE2);
    shader->setInt("roughTex", 2);

    metalTex->bind(GL_TEXTURE3);
    shader->setInt("metalTex", 3);

    aoTex->bind(GL_TEXTURE4);
    shader->setInt("aoTex", 4);

    model->draw();
}
