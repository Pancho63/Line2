#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    WindowP Wind;
    Wind.showFullScreen();
    Wind.show();

    return a.exec();
}
