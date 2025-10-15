cmake_minimum_required(VERSION 3.15)
project(jungizzRenderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)   # Qt MOC (Q_OBJECT 매크로) 자동 처리
set(CMAKE_AUTORCC ON)   # Qt 리소스 파일 자동 처리 (qrc)
set(CMAKE_AUTOUIC ON)   # Qt UI 파일 자동 처리

# 패키지 찾기 
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)
find_package(assimp REQUIRED)

qt_standard_project_setup()

# 실행 파일 생성
qt_add_executable(${PROJECT_NAME} 
    source/main.cpp
    source/scene.cpp
    source/MainWindow.cpp
    source/GLWidget.cpp
    source/core/Camera.cpp
    source/core/Shader.cpp
    source/core/Texture.cpp
    source/core/Mesh.cpp
    source/core/Model.cpp
    source/core/ScreenQuadMesh.cpp
    source/scenes/PBR_Scene.cpp
    source/scenes/SSSSS_Scene.cpp
)

# include 경로 설정
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source
        ${CMAKE_SOURCE_DIR}/dependencies/include
)


# 라이브러리 연결
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
        assimp::assimp
)

# 컴파일 옵션
# macOS에서 OpenGL이 더 이상 권장되지 않아서 뜨는 경고 무시
add_definitions(-DGL_SILENCE_DEPRECATION)

# 빌드 시 리소스, 쉐이더 코드를 자동으로 build/로 카피하도록 함
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shader
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shader
)