#include "Scene.h"

Scene::~Scene() {
    delete model;
    delete diffTex;
    delete normTex;
    delete roughTex;
    delete metalTex;
    delete aoTex;
}

void Scene::loadModel(const QString& path) {
    if(model) delete model;

    model = new Model(path.toStdString());
    if (!model) {
        qWarning() << "Failed to load model:" << path;
        return;
    }
}

void Scene::loadTexture(const QString& type, const QString& path) {
    Texture** tex = nullptr;
    if(type == "albedo") tex = &diffTex;
    else if(type == "normal") tex = &normTex;
    else if(type == "roughness") tex = &roughTex;
    else if(type == "metal") tex = &metalTex;
    else if(type == "ao") tex = &aoTex;
    
    if (*tex) {
        delete *tex;
        *tex = nullptr;
    }
    
    *tex = new Texture();

    if(path == "none"){
        glm::vec4 defaultColor;
        if (type == "albedo") defaultColor = glm::vec4(1, 1, 1, 1);          // 흰색
        else if (type == "normal") defaultColor = glm::vec4(0.5, 0.5, 1, 1); // default normal
        else if (type == "roughness") defaultColor = glm::vec4(1, 1, 1, 1);  // rough=1
        else if (type == "metal") defaultColor = glm::vec4(0, 0, 0, 1);      // 금속=0
        else if (type == "ao") defaultColor = glm::vec4(1, 1, 1, 1);         // AO=1

        unsigned char data[4] = {
            static_cast<unsigned char>(defaultColor.r * 255), 
            static_cast<unsigned char>(defaultColor.g * 255), 
            static_cast<unsigned char>(defaultColor.b * 255), 
            static_cast<unsigned char>(defaultColor.a * 255)
        };
        (*tex)->create(data, 1, 1);
    }
    else if (!(*tex)->loadFromFile(path.toStdString())) {
        qWarning() << "Failed to load texture:" << path;
        delete tex;
        tex = nullptr;
        return;
    }
}

void Scene::setCameraTarget(glm::vec3 target){
    camera = new Camera();
    camera->setTarget(target);
}
