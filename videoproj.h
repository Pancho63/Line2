#ifndef VIDEOPROJ_H
#define VIDEOPROJ_H

#include <QDialog>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QPen>
#include <QKeyEvent>
#include <QCheckBox>
#include "videoproj.h"
#include <QWindow>
#include <QGraphicsView>
//#include <QDesktopWidget>
#include <QCoreApplication>


class VideoProj : public QWidget
{
    Q_OBJECT

public:
    VideoProj();
//    ~VideoProj();
virtual void closeEvent(QCloseEvent*);

private:

    QGraphicsView *sview;
    QGraphicsRectItem *srect1;
    QGraphicsEllipseItem *sellipse1;
    QGraphicsRectItem *srect2;
    QGraphicsEllipseItem *sellipse2;
    QGraphicsScene *sm_graphicsScene;
    QGraphicsPixmapItem *spix;
    QPixmap spict;

public slots:
    void Update2();
    void addPix(int);
    void removePix();

};

#endif // VIDEOPROJ_H
