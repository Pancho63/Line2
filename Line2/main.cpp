#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    WindowP Wind;
    Wind.show();

    return a.exec();
}
