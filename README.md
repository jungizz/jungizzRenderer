# jungizzRenderer
### Rendering Tool (WIP)<img width="1161" height="860" alt="image" src="https://github.com/user-attachments/assets/c0bda827-3ed9-489d-b120-3bb3aa8f39d4" />


## 📌 Overview
- A **Qt OpenGL rendering sandbox** for experimenting with various scenes (BRDF, SSSSS, etc.) in one place.
- Comes with an interactive **Qt UI** for controlling scene settings in real time.
- *Developed as a personal learning project, focusing on exploring rendering techniques and engine structure design.*

## 🛠️ Tech Stack
- **C++17**
- **Qt6(OpenGL 4.1 core + Widgets)** – UI systems and OpenGL context management
- **GLM** – math library
- **Assimp** – model loading
- **CMake** - build system

## ⚙️ Build & Run
### macOS
1. Clone the repository
```bash
git clone https://github.com/jungizz/jungizzRenderer.git
cd jungizzRenderer
```

2. Configure (Replace /path/to/... with your actual installation path)
```bash
cmake -B build -S . \
  -DCMAKE_PREFIX_PATH=/path/to/Qt/6.9.2/macos/lib/cmake/Qt6
```

3. Build
```bash
cmake --build build
```

4. Run
```bash
cd build
./jungizzRenderer
```

### windows
1. Clone the repository
```bash
git clone https://github.com/jungizz/jungizzRenderer.git
cd jungizzRenderer
```

2. Configure (Replace /path/to/... with your actual installation path)
```bash
cmake -B build -S . ^
  -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake ^
  -G "Visual Studio 17 2022" -A x64 ^
  -DQt6_DIR=/path/to/Qt/6.9.2/msvc2022_64/lib/cmake/Qt6 ^
  -Dassimp_DIR=/path/to/vcpkg/installed/x64-windows/share/assimp ^
  -DCMAKE_PREFIX_PATH=/path/to/Qt/6.9.2/msvc2022_64
```

3. Build
```bash
cmake --build build --config Debug
```

4. Copy required DLLs
```bash
cd build\Debug
/path/to/Qt/6.9.2/msvc2022_64/bin/windeployqt.exe jungizzRenderer.exe
```

5. Run
```bash
jungizzRenderer.exe
```

## 📂 Project Structure
```
source/
├── GLWidget.h/cpp            # QOpenGLWidget 파생 클래스
├── MainWindow.h/cpp          # Qt 메인 윈도우, UI 컨트롤 패널
├── Scene.h/cpp               # Scene 기본 클래스 (모델/텍스처/카메라/라이트 관리)
│
├── core/
│   ├── Mesh.h/cpp
│   ├── ScreenQuadMesh.h/cpp
│   ├── Model.h/cpp
│   ├── Texture.h/cpp
│   ├── Camera.h/cpp
│   ├── Light.h/cpp
│   ├── Shader.h/cpp
│   └── Framebuffer.h
│
├── scenes/                   # 개별 씬 구현
│   ├── PBR_Scene.h/cpp
│   └── SSSSS_Scene.h/cpp
│
└── main.cpp                  # 프로그램 진입점
```

