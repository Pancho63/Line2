#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUdpSocket>
#include "oscpkt.hh"
#include "sacn/consts.h"
#include "sacn/sacnlistener.h"
#include "sacn/sacnsocket.h"
#include "sacn/sacnuniverselistmodel.h"
#include "sacn/streamcommon.h"
#include "sacn/streamingacn.h"
#include "sacn/ACNShare/CID.h"
#include "sacn/ACNShare/defpack.h"
#include "sacn/ACNShare/deftypes.h"
#include "sacn/ACNShare/ipaddr.h"
#include "sacn/ACNShare/tock.h"
#include "sacn/ACNShare/VHD.h"
#include <QTextStream>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QPen>
#include <QKeyEvent>
#include <QCheckBox>
//#include "videoproj.h"
#include <QWindow>
#include <QGraphicsView>
#include <QCoreApplication>
#include <QNetworkInterface>


using namespace oscpkt;


class WindowP : public QWidget
{
    Q_OBJECT

public:
     WindowP();

private:
    QUdpSocket  *udpSocket;
    QNetworkInterface interface;
    QSharedPointer<sACNListener> listener;
    void setupNetworkInterface();
    void processDMXData();

    QVector<int> dmxData;
    QGraphicsRectItem *rect1;
    QGraphicsEllipseItem *ellipse1;
    QGraphicsRectItem *rect2;
    QGraphicsEllipseItem *ellipse2;
    QGraphicsScene *m_graphicsScene;
    QGraphicsPixmapItem *pix;
    QPixmap pict;

    QGraphicsView *view;

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
