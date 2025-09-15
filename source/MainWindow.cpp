#include "MainWindow.h"
#include "GLWidget.h"
#include "scenes/PBR_Scene.h"
#include "scenes/SSSSS_Scene.h"

#include <QCoreApplication>
#include <QWidget>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QColorDialog>
#include <QSlider>
#include <QDoubleSpinBox>

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    // 중앙 gl위젯 배치
    m_glWidget = new GLWidget(this);
    setCentralWidget(m_glWidget);

    // 우측에 컨트롤 패널을 담을 dock 생성
    QDockWidget *dock = new QDockWidget("Scene Controls", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock); 

    // dock(부모)에 컨테이너 배치
    QWidget *dockContents = new QWidget(dock); dock->setWidget(dockContents);

    // 컨테이너(부모)에 수직 구조 레이아웃 배치 (ui가 들어갈 곳)
    QVBoxLayout *mainLayout = new QVBoxLayout(dockContents); dockContents->setLayout(mainLayout); 
 
    // --- 1. Scene 변경 ---
    m_sceneCombo = new QComboBox; mainLayout->addWidget(m_sceneCombo); 
    m_sceneCombo->addItem("PBR Scene");
    m_sceneCombo->addItem("SSSSS Scene");

    connect(m_sceneCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onSceneChanged);

    // --- 2. 모델 및 텍스처 로드 버튼 ---
    m_modelBtn = new QPushButton(tr("Load Model...")); mainLayout->addWidget(m_modelBtn);
    connect(m_modelBtn, &QPushButton::clicked, this, &MainWindow::onLoadModel);

    // --- 3. 라이트 속성 폼 ---
    QFormLayout *lightForm = new QFormLayout; mainLayout->addLayout(lightForm);

    // 3-1. Color: 버튼 + 스와치
    QHBoxLayout *colorRow = new QHBoxLayout; lightForm->addRow(tr("Light Color"), colorRow);

    m_colorBtn = new QPushButton(tr("Choose Color...")); colorRow->addWidget(m_colorBtn);
    connect(m_colorBtn, &QPushButton::clicked, this, &MainWindow::onLightColorClicked);

    m_colorSwatch = new QLabel; colorRow->addWidget(m_colorSwatch);
    m_colorSwatch->setFixedSize(24, 24);
    m_colorSwatch->setFrameShape(QFrame::Box);
    updateColorSwatch(Qt::white); // 초기값

    // 3-2. Position: X, Y, Z 
    QVBoxLayout *lightPosCol = new QVBoxLayout; lightForm->addRow(tr("Light Position"), lightPosCol);

    QHBoxLayout *lightXRow = new QHBoxLayout; lightPosCol->addLayout(lightXRow);
    QHBoxLayout *lightYRow = new QHBoxLayout; lightPosCol->addLayout(lightYRow);
    QHBoxLayout *lightZRow = new QHBoxLayout; lightPosCol->addLayout(lightZRow);

    QLabel *lightXText = new QLabel; lightXRow->addWidget(lightXText); 
    lightXText->setText("X");
    QLabel *lightYText = new QLabel; lightYRow->addWidget(lightYText); 
    lightYText->setText("Y");
    QLabel *lightZText = new QLabel; lightZRow->addWidget(lightZText); 
    lightZText->setText("Z");

    m_lightXSlider = new QSlider(Qt::Horizontal); lightXRow->addWidget(m_lightXSlider);
    m_lightXSlider->setRange(-1000, 1000); 
    m_lightYSlider = new QSlider(Qt::Horizontal); lightYRow->addWidget(m_lightYSlider);
    m_lightYSlider->setRange(-1000, 1000); 
    m_lightZSlider = new QSlider(Qt::Horizontal); lightZRow->addWidget(m_lightZSlider);
    m_lightZSlider->setRange(-1000, 1000); 
    m_lightXSlider->setValue(0.0); m_lightYSlider->setValue(0.0); m_lightZSlider->setValue(100.0); // 초기값

    m_lightXSpin = new QDoubleSpinBox; lightXRow->addWidget(m_lightXSpin);
    m_lightXSpin->setRange(-1000, 1000); m_lightXSpin->setSingleStep(0.1); 
    m_lightYSpin = new QDoubleSpinBox; lightYRow->addWidget(m_lightYSpin);
    m_lightYSpin->setRange(-1000, 1000); m_lightYSpin->setSingleStep(0.1); 
    m_lightZSpin = new QDoubleSpinBox; lightZRow->addWidget(m_lightZSpin);
    m_lightZSpin->setRange(-1000, 1000); m_lightZSpin->setSingleStep(0.1); 
    m_lightXSpin->setValue(0.0); m_lightYSpin->setValue(0.0); m_lightZSpin->setValue(100.0); // 초기값

    // slider -> spinbox
    connect(m_lightXSlider, &QSlider::valueChanged, 
            [this](int value){ m_lightXSpin->setValue(static_cast<double>(value)); });
    connect(m_lightYSlider, &QSlider::valueChanged, 
            [this](int value){ m_lightYSpin->setValue(static_cast<double>(value)); });
    connect(m_lightZSlider, &QSlider::valueChanged, 
            [this](int value){ m_lightZSpin->setValue(static_cast<double>(value)); });

    // spinbox -> slider
    connect(m_lightXSpin, &QDoubleSpinBox::valueChanged, 
            [this](double value){ m_lightXSlider->setValue(static_cast<int>(value)); });
    connect(m_lightYSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            [this](double value){ m_lightYSlider->setValue(static_cast<int>(value)); });
    connect(m_lightZSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
            [this](double value){ m_lightZSlider->setValue(static_cast<int>(value)); });

    connect(m_lightXSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double){ onLightPositionChanged(); });
    connect(m_lightYSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double){ onLightPositionChanged(); });
    connect(m_lightZSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double){ onLightPositionChanged(); });

    
    // 3-3. Intensity
    QHBoxLayout *lightValRow = new QHBoxLayout; lightForm->addRow(tr("Light Intensity"),lightValRow);

    m_lightValSlider = new QSlider(Qt::Horizontal); lightValRow->addWidget(m_lightValSlider);
    m_lightValSlider->setRange(0, 100);
    m_lightValSlider->setValue(30.0);

    m_lightValSpin = new QDoubleSpinBox; lightValRow->addWidget(m_lightValSpin);
    m_lightValSpin->setRange(0, 100); m_lightValSpin->setSingleStep(1.0); 
    m_lightValSpin->setValue(30.0);

    // slider -> spinbox
    connect(m_lightValSlider, &QSlider::valueChanged, 
            [this](int value){ m_lightValSpin->setValue(static_cast<double>(value)); });

    // spinbox -> slider
    connect(m_lightValSpin, &QDoubleSpinBox::valueChanged, 
            [this](double value){ m_lightValSlider->setValue(static_cast<int>(value)); });

    connect(m_lightValSlider, &QSlider::valueChanged, this, &MainWindow::onLightIntensityChanged);

    onSceneChanged(0);
}

void MainWindow::onSceneChanged(int index){
    qDebug() << "Scene changed to" << index;
    
    switch(index){
        case 0: m_glWidget->setScene(new PBR_Scene()); break;
        case 1: m_glWidget->setScene(new SSSSS_Scene()); break;
    }
    m_glWidget->update();
}

void MainWindow::onLoadModel(){
    QString modelPath = QFileDialog::getOpenFileName(this, tr("Open model"),
                                                QCoreApplication::applicationDirPath(), 
                                                tr("OBJ Files (*.obj);;GLTF Files (*.gltf *.glb);;All Files (*)"));
    if (modelPath.isEmpty()) return;

    QString relativeModelPath = QDir(QCoreApplication::applicationDirPath()).relativeFilePath(modelPath);
    qDebug() << "Load model:" << relativeModelPath;
    m_glWidget->setModel(relativeModelPath);

    // 텍스처 자동 검색
    QFileInfo modelInfo(modelPath);
    QString baseName = modelInfo.completeBaseName(); // "spaceHelmet" (확장자 제거)
    QString dirPath = modelInfo.absolutePath();

    QStringList textureTypes = {"albedo", "normal", "roughness", "metal", "ao"};
    QStringList extensions = {"png", "jpg", "jpeg"};

    for(const QString& type : textureTypes){
        QString texPath;
        for(const QString& ext : extensions){
            QString tempPath = dirPath + "/" + baseName + "_" + type + "." + ext;
            if(QFile::exists(tempPath)){
                texPath = tempPath;
                break;
            }
        }
        if(!texPath.isEmpty()){
            QString relativeTexPath = QDir(QCoreApplication::applicationDirPath()).relativeFilePath(texPath);
            qDebug() << "Load Texture:" << relativeTexPath;
            m_glWidget->setTexture(type, relativeTexPath);
        }
    }

    m_glWidget->update();
}

void MainWindow::updateColorSwatch(const QColor &c){
    QString style = QString("background-color: %1; border: 1px solid gray;").arg(c.name()); // 작은 색상 박스 스타일
    m_colorSwatch->setStyleSheet(style);
}

void MainWindow::onLightColorClicked(){
    QColor c = QColorDialog::getColor(Qt::white, this, tr("Select Light Color"));
    if (!c.isValid()) return;
    updateColorSwatch(c);
    glm::vec3 col(c.redF(), c.greenF(), c.blueF());
    m_glWidget->setLightColor(col);
    m_glWidget->update();
}

void MainWindow::onLightPositionChanged(){
    glm::vec3 pos((float)m_lightXSpin->value(), (float)m_lightYSpin->value(), (float)m_lightZSpin->value());
    m_glWidget->setLightPosition(pos);
    m_glWidget->update();
}

void MainWindow::onLightIntensityChanged(int value){
    m_glWidget->setLightIntensity((float)value/10.0f);
    m_glWidget->update();
}