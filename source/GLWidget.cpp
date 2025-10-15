#include "GLWidget.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_scene(nullptr){}

GLWidget::~GLWidget(){
    if(m_scene) delete m_scene;
}

void GLWidget::setScene(Scene* scene){
    if(m_scene) delete m_scene;
    m_scene = scene;

    if(context()) { // context가 생긴 이후에만 
        m_scene->init(glm::ivec2(m_width, m_height), this);
    }
}

void GLWidget::setModel(const std::string& path){
    if(m_scene) m_scene->loadModel(path);
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
        m_scene->init(glm::ivec2(m_width, m_height), this);
    }
}

void GLWidget::resizeGL(int w, int h){
    qreal dpr = devicePixelRatioF();
    m_width = w * dpr;
    m_height = h * dpr;    
    if(m_scene) m_scene->resizeFBO(glm::ivec2(m_width, m_height));
}

void GLWidget::paintGL(){
    if(m_scene) m_scene->render(glm::ivec2(m_width, m_height));
}

// ========== 이벤트 전달 ==========

void GLWidget::mousePressEvent(QMouseEvent *event) {
    int button = (event->button() == Qt::LeftButton) ? 0 : 
                 (event->button() == Qt::RightButton) ? 1 : 2;
    if (m_scene && m_scene->getCamera()) 
        m_scene->getCamera()->mousePressProcess(button, 1, event->pos().x(), event->pos().y());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    int button = (event->button() == Qt::LeftButton) ? 0 : 
                 (event->button() == Qt::RightButton) ? 1 : 2;
    if (m_scene && m_scene->getCamera()) 
        m_scene->getCamera()->mousePressProcess(button, 0, event->pos().x(), event->pos().y());
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (m_scene && m_scene->getCamera()) 
        m_scene->getCamera()->mouseMoveProcess(event->pos().x(), event->pos().y());
    update();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    if (m_scene && m_scene->getCamera()) 
        m_scene->getCamera()->scrollProcess(event->angleDelta().y()/120.0f); // 한 칸 = ±120
    update();
}
