#include "GLWidget.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_scene(nullptr), m_camera(nullptr){}

GLWidget::~GLWidget(){
    if(m_scene) delete m_scene;
}

void GLWidget::setScene(Scene* scene){
    if(m_scene) delete m_scene;
    m_scene = scene;

    if(context()) { // context가 생긴 이후에만 
        m_scene->init();
        m_camera = m_scene->getCamera();
    }
}

void GLWidget::setModel(const QString& path){
    if(m_scene) m_scene->loadModel(path);
}

void GLWidget::setTexture(const QString& type, const QString& path){
    if(m_scene) m_scene->loadTexture(type, path);
}

void GLWidget::setLightColor(glm::vec3 color){
    if(m_scene) m_scene->getLight()->color = color;
}

void GLWidget::setLightPosition(glm::vec3 pos){
    if(m_scene) m_scene->getLight()->position = pos;
}

void GLWidget::setLightIntensity(float intensity){
    if(m_scene) m_scene->getLight()->intensity = intensity;
}

void GLWidget::initializeGL(){
    initializeOpenGLFunctions();
    if(m_scene) {
        m_scene->init();
        m_camera = m_scene->getCamera();
    }
}

void GLWidget::resizeGL(int w, int h){
    qreal dpr = devicePixelRatioF();
    m_width = w * dpr;
    m_height = h * dpr;    
    glViewport(0, 0, m_width, m_height);
}

void GLWidget::paintGL(){
    if(m_scene) m_scene->render(glm::ivec2(m_width, m_height));
}

// ========== 이벤트 전달 ==========

void GLWidget::mousePressEvent(QMouseEvent *event) {
    int button = (event->button() == Qt::LeftButton) ? 0 : 
                 (event->button() == Qt::RightButton) ? 1 : 2;
    if (m_camera) m_camera->mousePressProcess(button, 1, event->pos().x(), event->pos().y());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    int button = (event->button() == Qt::LeftButton) ? 0 : 
                 (event->button() == Qt::RightButton) ? 1 : 2;
    if (m_camera) m_camera->mousePressProcess(button, 0, event->pos().x(), event->pos().y());
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (m_camera) m_camera->mouseMoveProcess(event->pos().x(), event->pos().y());
    update();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    if (m_camera) m_camera->scrollProcess(event->angleDelta().y()/120.0f); // 한 칸 = ±120
    update();
}
