#include "videoproj.h"


extern qreal master[5];
extern qreal X1[5];
extern qreal X2[5];
extern qreal Y1[5];
extern qreal Y2[5];
extern qreal R[5];
extern qreal rouge[5];
extern qreal vert[5];
extern qreal bleu[5];
extern qreal epais[5];

VideoProj::VideoProj() : QWidget()
{
    this->setCursor(Qt::BlankCursor);
    sm_graphicsScene = new QGraphicsScene(this);
    sm_graphicsScene->setSceneRect(0, 0, size().width(), size().height() );
    sview = new QGraphicsView(this);

    sview->setScene(sm_graphicsScene);
    sview->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    sview->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    sview->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    sview->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    srect1 = new QGraphicsRectItem();
    srect2 = new QGraphicsRectItem();
    sellipse1= new QGraphicsEllipseItem();
    sellipse2= new QGraphicsEllipseItem();
    spix = new QGraphicsPixmapItem();

    sm_graphicsScene->addItem(srect1);
    sm_graphicsScene->addItem(srect2);
    sm_graphicsScene->addItem(sellipse1);
    sm_graphicsScene->addItem(sellipse2);
    sm_graphicsScene->addItem(spix);
}

void VideoProj::Update2()
{

        srect1->setRect(QRectF((X1[0]*size().width()/65280), (Y1[0]*size().height()/65280), (X2[0]*size().width()/65280), (Y2[0]*size().height()/65280)));
        srect1->setPen(QPen(QColor(rouge[0], vert[0], bleu[0], master[0]), epais[0]));
        srect1->setTransformOriginPoint(srect1->boundingRect().center());
        srect1->setRotation(R[0]);

        srect2->setRect(QRectF(((size().width()-X1[1]*size().width()/65280)-X2[1]*size().width()/65280), ((size().height()-Y1[1]*size().height()/65280)-Y2[1]*size().height()/65280), (X2[1]*size().width()/65280), (Y2[1]*size().height()/65280)));
        srect2->setPen(QPen(QColor(rouge[1], vert[1], bleu[1], master[1]), epais[1]));
        srect2->setTransformOriginPoint(srect2->boundingRect().center());
        srect2->setRotation(R[1]);

        sellipse1->setRect(QRectF(((size().width()-X1[2]*size().width()/65280)-X2[2]*size().width()/65280), (Y1[2]*size().height()/65280), (X2[2]*size().width()/65280), (Y2[2]*size().height()/65280)));
        sellipse1->setPen(QPen(QColor(rouge[2], vert[2], bleu[2], master[2]), epais[2]));
        sellipse1->setTransformOriginPoint(sellipse1->boundingRect().center());
        sellipse1->setRotation(R[2]);

        sellipse2->setRect(QRectF((X1[3]*size().width()/65280), ((size().height()-Y1[3]*size().height()/65280)-Y2[3]*size().height()/65280), (X2[3]*size().width()/65280), (Y2[3]*size().height()/65280)));
        sellipse2->setPen(QPen(QColor(rouge[3], vert[3], bleu[3], master[3]), epais[3]));
        sellipse2->setTransformOriginPoint(sellipse2->boundingRect().center());
        sellipse2->setRotation(R[3]);

        spix->setTransformationMode(Qt::SmoothTransformation);
        qreal xPix = X1[4]*size().width();
        qreal yPix = Y1[4]*size().height();
        qreal hPix = X2[4]*size().width()/127;
        qreal vPix = Y2[4]*size().height()/127;
        spix->setOpacity(master[4]/255);
        QTransform trans;
        trans.setMatrix(hPix/65280,0,0,0,vPix/65280,0,xPix/65280,yPix/65280,1);
        spix->setTransform(trans);
        spix->setTransformOriginPoint(spix->boundingRect().center());
        spix->setRotation(R[4]);
}

void VideoProj::addPix(int no)
{
    switch (no)
    {
    case 0:
        spict = QPixmap();
        spix->setPixmap (spict);
        sm_graphicsScene->addItem(spix);
             break;
     case 1:
    spict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/1.png");
    if (spict.isNull()) break;
    spict =  spict.scaled(size().width(), size().height());
    spix->setPixmap (spict);
    sm_graphicsScene->addItem(spix);
        break;
    case 2:
   spict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/2.png");
   if (spict.isNull()) break;
   spict =  spict.scaled(size().width(), size().height());
   spix->setPixmap (spict);
   sm_graphicsScene->addItem(spix);
       break;
    case 3:
   spict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/3.png");
   if (spict.isNull()) break;
   spict =  spict.scaled(size().width(), size().height());
   spix->setPixmap (spict);
   sm_graphicsScene->addItem(spix);
       break;
    case 4:
   spict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/4.png");
   if (spict.isNull()) break;
   spict =  spict.scaled(size().width(), size().height());
   spix->setPixmap (spict);
   sm_graphicsScene->addItem(spix);
       break;
    case 5:
   spict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/5.png");
   if (spict.isNull()) break;
   spict =  spict.scaled(size().width(), size().height());
   spix->setPixmap (spict);
   sm_graphicsScene->addItem(spix);
       break;
    case 6:
   spict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/6.png");
   if (spict.isNull()) break;
   spict =  spict.scaled(size().width(), size().height());
   spix->setPixmap (spict);
   sm_graphicsScene->addItem(spix);
       break;
   case 7:
  spict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/7.png");
  if (spict.isNull()) break;
  spict =  spict.scaled(size().width(), size().height());
  spix->setPixmap (spict);
  sm_graphicsScene->addItem(spix);
      break;
   case 8:
  spict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/8.png");
  if (spict.isNull()) break;
  spict =  spict.scaled(size().width(), size().height());
  spix->setPixmap (spict);
  sm_graphicsScene->addItem(spix);
      break;
   case 9:
  spict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/9.png");
  if (spict.isNull()) break;
  spict =  spict.scaled(size().width(), size().height());
  spix->setPixmap (spict);
  sm_graphicsScene->addItem(spix);
      break;
   case 10:
  spict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/10.png");
  if (spict.isNull()) break;
  spict =  spict.scaled(size().width(), size().height());
  spix->setPixmap (spict);
  sm_graphicsScene->addItem(spix);
      break;
    }
}

void VideoProj::removePix()
{
    sm_graphicsScene->removeItem(spix);
}
/*
VideoProj::~VideoProj()
{
    delete VideoProj;
}
*/
void VideoProj::closeEvent(QCloseEvent *)
{
    qApp->quit();
}
