#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "Scene.h"
#include "core/Camera.h"

class GLWidget:public QOpenGLWidget, protected QOpenGLFunctions{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

    void setScene(Scene* scene);
    void setModel(const QString& path);
    void setTexture(const QString& type, const QString& path);
    void setLightColor(glm::vec3 color);
    void setLightPosition(glm::vec3 pos);
    void setLightIntensity(float intensity);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    Scene *m_scene;
    Camera *m_camera;
    int m_width, m_height;
};