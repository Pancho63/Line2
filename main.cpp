#include <QApplication>
#include "mainwindow.h"
#include "dmxreceiver.h"

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    WindowP window;
    window.show();

    return app.exec();
}
