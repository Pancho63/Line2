#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    WindowP window;
    window.showFullScreen();

    return app.exec();
}
