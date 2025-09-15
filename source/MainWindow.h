#pragma once
#include <QMainWindow>

class GLWidget;
class QComboBox;
class QPushButton;
class QLabel;
class QSlider;
class QDoubleSpinBox;

class MainWindow: public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void onSceneChanged(int index);
    void onLoadModel();
    void onLightColorClicked();
    void onLightPositionChanged();
    void onLightIntensityChanged(int value);

private:
    GLWidget *m_glWidget;
    
    QComboBox *m_sceneCombo;
    QPushButton *m_modelBtn;
    QPushButton *m_textureBtn;

    QPushButton *m_colorBtn;
    QLabel *m_colorSwatch;

    QSlider *m_lightXSlider;
    QSlider *m_lightYSlider;
    QSlider *m_lightZSlider;
    QDoubleSpinBox *m_lightXSpin;
    QDoubleSpinBox *m_lightYSpin;
    QDoubleSpinBox *m_lightZSpin;

    QSlider *m_lightValSlider;
    QDoubleSpinBox *m_lightValSpin;

    void updateColorSwatch(const QColor &c);
};

