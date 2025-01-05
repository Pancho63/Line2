#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCheckBox>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QKeyEvent>
#include <QPen>
#include <QTextStream>
#include <QUdpSocket>
#include "oscpkt.hh"
#include "sacn/ACNShare/CID.h"
#include "sacn/ACNShare/VHD.h"
#include "sacn/ACNShare/defpack.h"
#include "sacn/ACNShare/ipaddr.h"
#include "sacn/ACNShare/tock.h"
#include "sacn/sacnlistener.h"
#include "sacn/streamingacn.h"
#include <QCoreApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QNetworkInterface>
#include <QWindow>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QPixmap>
#include <QShortcut>
#include <QApplication>
#include <QCursor>

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

protected:
    void keyPressEvent(QKeyEvent *event) override;

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
