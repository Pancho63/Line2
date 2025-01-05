#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QUdpSocket>
#include <QNetworkInterface>
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

#include <QWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QPixmap>
#include <QCursor>

#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QKeyEvent>
#include <QShortcut>


using namespace oscpkt;

class WindowP : public QWidget
{
    Q_OBJECT

public:
    WindowP();

private:
    QUdpSocket *udpSocket;
    QNetworkInterface interface;
    QSharedPointer<sACNListener> listener;
    QVector<int> dmxData;

    QGraphicsRectItem *rect1;
    QGraphicsRectItem *rect2;
    QGraphicsEllipseItem *ellipse1;
    QGraphicsEllipseItem *ellipse2;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsPixmapItem *pix;
    QPixmap pict;


    void setupNetworkInterface();
    void processDMXData();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void processPendingDatagrams();
    void ligneUpdate();

    void masterLevel(int, int);
    void red(int, int);
    void green(int, int);
    void blue(int, int);
    void redSacn(int, int);
    void greenSacn(int, int);
    void blueSacn(int, int);
    void pan(int, int);
    void tilt(int, int);
    void largeur(int, int);
    void hauteur(int, int);
    void rotate(int, int);
    void thickness(int, int);
    void picture(int);
    void pictureSacn(int);

public slots:
    void onLevelsChanged();
};

#endif // MAINWINDOW_H
