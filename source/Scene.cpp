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
    if (!(*tex)->loadFromFile(path.toStdString())) {
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
