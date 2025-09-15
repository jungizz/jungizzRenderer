# jungizzRenderer
### Rendering Tool (WIP)<img width="1161" height="860" alt="image" src="https://github.com/user-attachments/assets/c0bda827-3ed9-489d-b120-3bb3aa8f39d4" />


## 📌 Overview
- **Renderer development playground** where different rendering scenes (BRDF, SSSSS, etc.) can be tested in a unified environment.  
- It is structured as a rendering tool with a **Qt-based UI**, allowing interactive adjustments of scene properties such as lighting, materials, and models.  


## 🛠️ Tech Stack
- **C++17**
- **Qt6** – UI framework and event handling
- **OpenGL** – real-time rendering
- **GLM** – math library
- **stb_image** – texture loading
- **Assimp** – model loading

## 📂 Project Structure
```
source/
├── GLWidget.h/cpp            # QOpenGLWidget 파생 클래스
├── MainWindow.h/cpp          # Qt 메인 윈도우, UI 컨트롤 패널
├── Scene.h/cpp               # Scene 기본 클래스 (모델/텍스처/카메라/라이트 관리)
│
├── core/
│   ├── Mesh.h/cpp
│   ├── Model.h/cpp
│   ├── Texture.h/cpp
│   ├── Camera.h/cpp
│   ├── Light.h/cpp
│   ├── Shader.h/cpp
│   └── Framebuffer.h
│
├── scenes/                   # 개별 씬 구현
│   ├── BRDF_Scene.h/cpp
│   └── SSSSS_Scene.h/cpp
│
└── main.cpp                  # 프로그램 진입점
```

