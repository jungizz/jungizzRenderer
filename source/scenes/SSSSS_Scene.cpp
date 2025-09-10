#include "SSSSS_Scene.h"
#include <iostream>

using namespace glm;

SSSSS_Scene::SSSSS_Scene()
    : shader(nullptr), camera(nullptr), model(nullptr),
      diffTex(nullptr), normTex(nullptr), roughTex(nullptr){}

SSSSS_Scene::~SSSSS_Scene() { 
    delete shader;
    delete camera;
    delete model;
    delete diffTex;
    delete normTex;
    delete roughTex;
}

void SSSSS_Scene::init(){
    glfwSwapInterval(1);

    //모델 로드
    model = new Model("resources/LPS_Head.obj");
    if(!model){
        std::cerr << "Failed to load the model!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // 텍스처 로드
    diffTex = new Texture();    
    if (!diffTex->loadFromFile("resources/LPS_lambertian.jpg")) {
        std::cerr << "Failed to load diffuse texture!\n";
    }

    normTex = new Texture();
    if (!normTex->loadFromFile("resources/LPS_NormalMap.png")) {
        std::cerr << "Failed to load normal texture!\n";
    }

    roughTex = new Texture();
    if (!roughTex->loadFromFile("resources/LPS_Roughness.png")) {
        std::cerr << "Failed to load roughness texture!\n";
    }

    // 셰이더 로드
    shader = new Shader("shader/vertex_shader.vert", "shader/fragment_shader.frag");

    // 카메라 세팅
    camera = new Camera();
    camera->setTarget(vec3(0.0f));

    // 라이트 세팅
    light = Light(vec3(3.0f, 3.0f, 10.0f), vec3(1.0f), 3.0f);
}

void SSSSS_Scene::render(const ivec2& framebufferSize){
    glViewport(0, 0, framebufferSize.x, framebufferSize.y);
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    shader->setMat4("modelMat", mat4(1.0f));
    shader->setMat4("viewMat", camera->getViewMatrix());
    shader->setMat4("projMat", camera->getProjectionMatrix((float)framebufferSize.x/framebufferSize.y));
    shader->setVec3("cameraPosition", camera->getPosition());

    shader->setVec3("lightPosition", light.position);
    shader->setVec3("lightColor", light.color * light.intensity);
    //shader->setVec3("ambientLight", vec3(0.0));

    diffTex->bind(GL_TEXTURE0);
    shader->setInt("diffTex", 0);

    normTex->bind(GL_TEXTURE1);
    shader->setInt("normTex", 1);

    roughTex->bind(GL_TEXTURE2);
    shader->setInt("roughTex", 2);
    glActiveTexture(GL_TEXTURE0);

    model->draw();
}
