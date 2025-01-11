#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUdpSocket>
#include <QNetworkInterface>
#include "oscpkt.hh"

#include <QApplication>
#include <QCoreApplication>

#include <QWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QPen>
#include <QPixmap>
#include <QCursor>

#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QKeyEvent>
#include <QShortcut>
#include <QScreen>


using namespace oscpkt;


class WindowP : public QWidget
{
    Q_OBJECT

public:
     WindowP();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QUdpSocket  *udpSocket;
    QNetworkInterface interface;

    void setupNetworkInterfaces();


    QVector<int> dmxData;

    QGraphicsRectItem *rect1;
    QGraphicsEllipseItem *ellipse1;
    QGraphicsRectItem *rect2;
    QGraphicsEllipseItem *ellipse2;
    QGraphicsPixmapItem *pix;

    QPixmap pict;
    QGraphicsScene *scene;
    QGraphicsView *view;

    bool dmxDataChanged; // Flag to indicate if DMX data has changed

private slots:
    void processPendingDatagrams();



    void ligneUpdate();

public slots:
    void masterLevel(int, int);
    void pan(int, int);
    void tilt(int, int);
    void largeur(int, int);
    void hauteur(int, int);
    void rotate(int, int);
    void red(int, int);
    void green(int, int);
    void blue(int, int);
    void redSacn(int, int);
    void greenSacn(int, int);
    void blueSacn(int, int);
    void thickness(int, int);
    void picture(int);
    void pictureSacn(int);
    void processDMXData();

     void onLevelsChanged();
     void checkAndProcessDMXData();

};

#endif // MAINWINDOW_H
