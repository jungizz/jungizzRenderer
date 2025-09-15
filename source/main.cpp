#include <QApplication>
#include <QSurfaceFormat>
#include "MainWindow.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    QSurfaceFormat fmt;
    //fmt.setDepthBufferSize(24);
    fmt.setVersion(4, 1); // OpenGL 4.1 요청
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);

    MainWindow win;
    win.resize(1280, 720);
    win.show();

    return app.exec();
}