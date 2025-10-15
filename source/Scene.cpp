#include "Scene.h"
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

Scene::~Scene() {
    delete model;
    delete diffTex;
    delete normTex;
    delete roughTex;
    delete metalTex;
    delete aoTex;
}

void Scene::initialize(QOpenGLFunctions_4_1_Core* f) {
    funcs = f;
    if (!funcs) {
        qWarning("Scene::initialize failed: funcs is null!");
    }
}

void Scene::beginPass(const glm::ivec2& size, bool useDepth){
    funcs->glViewport(0, 0, size.x, size.y);
    funcs->glClearColor(0, 0, 0, 0);
    if(useDepth) funcs->glEnable(GL_DEPTH_TEST);
    else funcs->glDisable(GL_DEPTH_TEST);
    funcs->glClear(GL_COLOR_BUFFER_BIT | (useDepth ? GL_DEPTH_BUFFER_BIT : 0));
}

void Scene::loadModel(const string& path) {
    if(model) delete model;

    model = new Model(path);
    if (!model) {
        qWarning() << "Failed to load model:" << path;
        return;
    }

    // 텍스처 자동 검색
    fs::path modelPath(path);
    string baseName = modelPath.stem().string(); // "spaceHelmet" (확장자 제거)
    fs::path dirPath = modelPath.parent_path();

    vector<string> textureTypes = {"albedo", "normal", "roughness", "metal", "ao"};
    vector<string> extensions = {"png", "jpg", "jpeg"};

    for(const string& type : textureTypes){
        string texPath;
        for(const string& ext : extensions){
            fs::path candidate = dirPath / (baseName + "_" + type + "." + ext);
            if(fs::exists(candidate)){
                texPath = candidate.string();
                break;
            }
        }
        if(!texPath.empty()){
            qDebug() << "Load Texture:" << texPath;
            loadTexture(type, texPath);
        }
        else{ // 텍스처 없는 경우 기본 대체 텍스처 세팅
            loadTexture(type, "none");
        }
    }
}

void Scene::loadTexture(const string& type, const string& path) {
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
    else if (!(*tex)->loadFromFile(path)) {
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

