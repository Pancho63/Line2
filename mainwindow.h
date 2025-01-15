#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QNetworkInterface>
#include <QUdpSocket>
#include "oscpkt.hh"
#include "sacn/ACNShare/CID.h"
#include "sacn/ACNShare/VHD.h"
#include "sacn/ACNShare/defpack.h"
#include "sacn/ACNShare/ipaddr.h"
#include "sacn/ACNShare/tock.h"
#include "sacn/sacnlistener.h"
#include "sacn/streamingacn.h"

#include <QApplication>
#include <QCoreApplication>

#include <QCursor>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPen>
#include <QPixmap>
#include <QWindow>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QKeyEvent>
#include <QScreen>
#include <QShortcut>
#include <QTextStream>

#include <chrono>
#include <fstream>
#include <string>
#include <thread>

using namespace oscpkt;

class WindowP : public QWidget
{
    Q_OBJECT

public:
    WindowP();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QUdpSocket *udpSocket;
    QNetworkInterface interface;
    QSharedPointer<sACNListener> listener;
    void setupNetworkInterfaces();
    void processDMXData();

    QVector<int> dmxData;
    QGraphicsRectItem *rect1;
    QGraphicsEllipseItem *ellipse1;
    QGraphicsRectItem *rect2;
    QGraphicsEllipseItem *ellipse2;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsPixmapItem *pix;
    QPixmap pict;

    //    VideoProj *videoproj;
private slots:
    void processPendingDatagrams();

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
    void ligneUpdate();

public slots:
    void onLevelsChanged();
};

#endif // MAINWINDOW_H
