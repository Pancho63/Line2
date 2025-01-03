#include "mainwindow.h"


#include <QDebug>
#include <QShortcut>
#include <QPixmap>
#include <QDir>


int         iarg;
int         iarg2;
std::string sarg;
qreal       master[5]=    {0, 0, 0, 0, 0};
qreal       X1[5]=        {0, 0, 0, 0, 0};
qreal       X2[5]=        {0, 0, 0, 0, 0};
qreal       Y1[5]=        {0, 0, 0, 0, 0};
qreal       Y2[5]=        {0, 0, 0, 0, 0};
qreal       R[5]=         {0, 0, 0, 0, 0};
qreal       rouge[5]=     {0, 0, 0, 0, 0};
qreal       vert[5]=      {0, 0, 0, 0, 0};
qreal       bleu[5]=      {0, 0, 0, 0, 0};
qreal       epais[5]=     {0, 0, 0, 0, 0};
int         channel =     61;
//bool        VpExist=      false;
QSharedPointer<sACNListener> listener;

WindowP::WindowP() :    QWidget(), dmxData(512, 0)

{
    setupNetworkInterface();

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(7003);
      connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));


    view = new QGraphicsView(this);
    m_graphicsScene = new QGraphicsScene(this);
    m_graphicsScene->setSceneRect(0, 0, size().width(), size().height() );

    view->setScene(m_graphicsScene);
    view->setVerticalScrollBarPolicy  ( Qt::ScrollBarAlwaysOff );
    view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    view->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    view->setRenderHints(QPainter::Antialiasing| QPainter::SmoothPixmapTransform);
    view->setAutoFillBackground(true);
    view->show();

    rect1 = new QGraphicsRectItem();
    rect2 = new QGraphicsRectItem();
    ellipse1= new QGraphicsEllipseItem();
    ellipse2= new QGraphicsEllipseItem();
    pix = new QGraphicsPixmapItem();

    m_graphicsScene->addItem(rect1);
    m_graphicsScene->addItem(rect2);
    m_graphicsScene->addItem(ellipse1);
    m_graphicsScene->addItem(ellipse2);
    m_graphicsScene->addItem(pix);

}

void WindowP::processPendingDatagrams()
{
PacketReader pr;
while (udpSocket->hasPendingDatagrams()) {
    oscpkt::Message *msg;
    QByteArray datagram;
    datagram.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(datagram.data(), datagram.size());
    pr.init(datagram.data(), datagram.size());


    while (pr.isOk() && (msg = pr.popMessage()) != 0) {

        if ((msg->match("/circ/level").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())         &&(channel<=iarg && iarg<=(channel+4)))   {masterLevel(iarg, iarg2);}
        if ((msg->match("/device/FOCUS/PAN").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())   &&(channel<=iarg && iarg<=(channel+4)))   {pan(iarg, iarg2);}
        if ((msg->match("/device/FOCUS/TILT").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())  &&(channel<=iarg && iarg<=(channel+4)))   {tilt (iarg, iarg2);}
        if ((msg->match("/device/BEAM/IRIS").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())   &&(channel<=iarg && iarg<=(channel+4)))   {largeur  (iarg, iarg2);}
        if ((msg->match("/device/BEAM/FOCUS").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())  &&(channel<=iarg && iarg<=(channel+4)))   {hauteur (iarg, iarg2);}
        if ((msg->match("/device/BEAM/FROST").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())  &&(channel<=iarg && iarg<=(channel+4)))   {rotate(iarg, iarg2);}
        if ((msg->match("/device/COLOUR/RED").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())  &&(channel<=iarg && iarg<=(channel+4)))   {red  (iarg, iarg2);}
        if ((msg->match("/device/COLOUR/GREEN").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())&&(channel<=iarg && iarg<=(channel+4)))   {green(iarg, iarg2);}
        if ((msg->match("/device/COLOUR/BLUE").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs()) &&(channel<=iarg && iarg<=(channel+4)))   {blue (iarg, iarg2);}
        if ((msg->match("/device/BEAM/ZOOM").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())   &&(channel<=iarg && iarg<=(channel+4)))   {thickness(iarg, iarg2);}
        if ((msg->match("/device/EFFECT/GOBO").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs()) &&(iarg=channel+4))             {picture(iarg2);}
           }
}
}


void WindowP::masterLevel (int ch, int value)
{
master[ch-channel] = value;
ligneUpdate();
}

void WindowP::pan (int ch, int value)
{
X1[ch-channel] = value;
ligneUpdate();
}

void WindowP::tilt (int ch, int value)
{
Y1[ch-channel] = value;
ligneUpdate();
}

void WindowP::largeur (int ch, int value)
{
X2[ch-channel] = value;
ligneUpdate();
}

void WindowP::hauteur (int ch, int value)
{
Y2[ch-channel] = value;
ligneUpdate();
}

void WindowP::rotate(int ch, int value)
{
R[ch-channel] = 360*value/65280;
ligneUpdate();
}

void WindowP::red(int ch, int value)
{
rouge[ch-channel] = ((value+1)/256);
ligneUpdate();
}

void WindowP::green(int ch, int value)
{
vert[ch-channel] = ((value+1)/256);
ligneUpdate();
}

void WindowP::blue(int ch, int value)
{
bleu[ch-channel] = ((value+1)/256);
ligneUpdate();
}

void WindowP::thickness(int ch, int value)
{
epais[ch-channel] = value*size().width()/650000;
ligneUpdate();
}

void WindowP::picture(int value)
{
    QString path = (QCoreApplication::applicationDirPath() + "/imageLine/");
    QDir dir(path);
    if (!dir.exists()) {dir.mkpath(".");}

    m_graphicsScene->removeItem(pix);
//    if (VpExist) videoproj->removePix();

 switch (value)
 {
 case 0:
     pict = QPixmap();
     pix->setPixmap (pict);
     m_graphicsScene->addItem(pix);
//     if (VpExist) videoproj->addPix(0);
          break;
 case 2944:
    pict = QPixmap();
    pix->setPixmap (pict);
    m_graphicsScene->addItem(pix);
//    if (VpExist) videoproj->addPix(0);
          break;
 case 8960:
     pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/1.png");
     if (pict.isNull()) break;
     pict =  pict.scaled(size().width(), size().height());
     pix->setPixmap (pict);
     m_graphicsScene->addItem(pix);
//     if (VpExist) videoproj->addPix(1);
             break;
  case 14848:
     pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/2.png");
     if (pict.isNull()) break;
     pict =  pict.scaled(size().width(), size().height());
     pix->setPixmap (pict);
     m_graphicsScene->addItem(pix);
//     if (VpExist) videoproj->addPix(2);
             break;
 case 20736:
    pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/3.png");
    if (pict.isNull()) break;
    pict =  pict.scaled(size().width(), size().height());
    pix->setPixmap (pict);
    m_graphicsScene->addItem(pix);
//    if (VpExist) videoproj->addPix(3);
            break;
 case 26624:
    pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/4.png");
    if (pict.isNull()) break;
    pict =  pict.scaled(size().width(), size().height());
    pix->setPixmap (pict);
    m_graphicsScene->addItem(pix);
//    if (VpExist) videoproj->addPix(4);
            break;
 case 32512:
    pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/5.png");
    if (pict.isNull()) break;
    pict =  pict.scaled(size().width(), size().height());
    pix->setPixmap (pict);
    m_graphicsScene->addItem(pix);
//    if (VpExist) videoproj->addPix(5);
            break;
 case 38400:
    pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/6.png");
    if (pict.isNull()) break;
    pict =  pict.scaled(size().width(), size().height());
    pix->setPixmap (pict);
    m_graphicsScene->addItem(pix);
//    if (VpExist) videoproj->addPix(6);
            break;
 case 44288:
    pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/7.png");
    if (pict.isNull()) break;
    pict =  pict.scaled(size().width(), size().height());
    pix->setPixmap (pict);
    m_graphicsScene->addItem(pix);
//    if (VpExist) videoproj->addPix(7);
            break;
 case 50176:
    pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/8.png");
    if (pict.isNull()) break;
    pict =  pict.scaled(size().width(), size().height());
    pix->setPixmap (pict);
    m_graphicsScene->addItem(pix);
//    if (VpExist) videoproj->addPix(8);
            break;
 case 56064:
    pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/9.png");
    if (pict.isNull()) break;
    pict =  pict.scaled(size().width(), size().height());
    pix->setPixmap (pict);
    m_graphicsScene->addItem(pix);
//    if (VpExist) videoproj->addPix(9);
            break;
 case 62208:
    pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/10.png");
    if (pict.isNull()) break;
    pict =  pict.scaled(size().width(), size().height());
    pix->setPixmap (pict);
    m_graphicsScene->addItem(pix);
//    if (VpExist) videoproj->addPix(10);
            break;
 }
 ligneUpdate();
// if (VpExist) videoproj->Update2();
}

void WindowP::ligneUpdate()
{
    rect1->setRect(QRectF((X1[0]*size().width()/65280), (Y1[0]*size().height()/65280), (X2[0]*size().width()/65280), (Y2[0]*size().height()/65280)));
    rect1->setPen(QPen(QColor(rouge[0], vert[0], bleu[0], master[0]), epais[0]));
    rect1->setTransformOriginPoint(rect1->boundingRect().center());
    rect1->setRotation(R[0]);

    rect2->setRect(QRectF(((size().width()-X1[1]*size().width()/65280)-X2[1]*size().width()/65280), ((size().height()-Y1[1]*size().height()/65280)-Y2[1]*size().height()/65280), (X2[1]*size().width()/65280), (Y2[1]*size().height()/65280)));
    rect2->setPen(QPen(QColor(rouge[1], vert[1], bleu[1], master[1]), epais[1]));
    rect2->setTransformOriginPoint(rect2->boundingRect().center());
    rect2->setRotation(R[1]);

    ellipse1->setRect(QRectF(((size().width()-X1[2]*size().width()/65280)-X2[2]*size().width()/65280), (Y1[2]*size().height()/65280), (X2[2]*size().width()/65280), (Y2[2]*size().height()/65280)));
    ellipse1->setPen(QPen(QColor(rouge[2], vert[2], bleu[2], master[2]), epais[2]));
    ellipse1->setTransformOriginPoint(ellipse1->boundingRect().center());
    ellipse1->setRotation(R[2]);

    ellipse2->setRect(QRectF((X1[3]*size().width()/65280), ((size().height()-Y1[3]*size().height()/65280)-Y2[3]*size().height()/65280), (X2[3]*size().width()/65280), (Y2[3]*size().height()/65280)));
    ellipse2->setPen(QPen(QColor(rouge[3], vert[3], bleu[3], master[3]), epais[3]));
    ellipse2->setTransformOriginPoint(ellipse2->boundingRect().center());
    ellipse2->setRotation(R[3]);

    pix->setTransformationMode(Qt::SmoothTransformation);
    qreal xPix = X1[4]*size().width();
    qreal yPix = Y1[4]*size().height();
    qreal hPix = X2[4]*size().width()/127;
    qreal vPix = Y2[4]*size().height()/127;
    pix->setOpacity(master[4]/255);
    QTransform trans;
    trans.setMatrix(hPix/65280,0,0,0,vPix/65280,0,xPix/65280,yPix/65280,1);
    pix->setTransform(trans);
    pix->setTransformOriginPoint(pix->boundingRect().center());
    pix->setRotation(R[4]);
}
void WindowP::setupNetworkInterface() {
    // Obtenir la liste de toutes les interfaces réseau
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    // Spécifiez le nom de l'interface réseau que vous souhaitez utiliser
    QString interfaceName = "enp2s0f1"; // Remplacez par le nom réel de votre interface réseau
    QNetworkInterface selectedInterface;

    // Trouvez l'interface réseau par son nom
    foreach (const QNetworkInterface &interface, interfaces) {
        if (interface.humanReadableName() == interfaceName) {
            selectedInterface = interface;
            break;
        }
    }

    // Utiliser l'interface sélectionnée avec sACNRxSocket
    if (selectedInterface.isValid()) {
        qDebug() << "Interface sélectionnée:" << selectedInterface.humanReadableName();

        // Créez une instance de sACNRxSocket
        sACNRxSocket rxSocket;

        // Définir l'interface réseau
        rxSocket.setNetworkInterface(selectedInterface);

        // Continuez avec les autres opérations sur rxSocket
        int universe = 1;  // universe in range 1-63999
        listener = sACNManager::getInstance()->getListener(universe);
        if (listener) {
            connect(listener.data(), SIGNAL(levelsChanged()), this, SLOT(onLevelsChanged()));
            qDebug() << "Listener created for universe" << universe;
        } else {
            qDebug() << "Failed to create listener for universe" << universe;
        }
    } else {
        qDebug() << "Interface non trouvée.";
    }
}

void WindowP::onLevelsChanged() {
    qDebug() << "Slot onLevelsChanged called!";

    // Mettre à jour le tableau dmxData avec les nouvelles valeurs
    for (int channel = 100; channel < 200; ++channel) {
        qDebug() << "tableau1 ";
        if (channel < 512) {
            qDebug() << "tableau2 ";
            qDebug() << "dmxData.size:" << dmxData.size();
             qDebug() << "listener.size:" << listener->mergedLevels().size();
            if (channel >= 0 && channel < dmxData.size() && channel < listener->mergedLevels().size()) {
                dmxData[channel] = listener->mergedLevels()[channel].level;
            } else {
                qWarning() << "Channel index out of bounds:" << channel;
            }
            qDebug() << "tableau3 ";
        }
    }
    qDebug() << "tableau mis à jour";
    processDMXData();
}

void WindowP::processDMXData() {
    // Traiter les données DMX pour les canaux entre 100 et 199
    for (int baseChannel = 100; baseChannel < 200; baseChannel += 20) {
        // Traiter les canaux 8 bits (100-103, 120-123, 140-143, etc.)
        for (int i = 0; i < 4; ++i) {
            int channel = baseChannel + i;
            if (channel < 512) {
                int level = dmxData[channel];
                qDebug() << "8-bit channel" << channel << "level:" << level;
                // Ajoutez votre traitement pour les canaux 8 bits ici
            }
        }

        // Traiter les canaux 16 bits (104-108, 124-128, 144-148, etc.)
        for (int i = 4; i < 9; i += 2) {
            int highChannel = baseChannel + i;
            int lowChannel = baseChannel + i + 1;
            if (highChannel < 512 && lowChannel < 512) {
                int highByte = dmxData[highChannel];
                int lowByte = dmxData[lowChannel];
                int combinedValue = (highByte << 8) | lowByte;
                qDebug() << "16-bit channels" << highChannel << "and" << lowChannel << "combined value:" << combinedValue;
                // Ajoutez votre traitement pour les canaux 16 bits ici
            }
        }

        // Traiter le canal 8 bits supplémentaire (109, 129, 149, etc.)
        int extraChannel = baseChannel + 9;
        if (extraChannel < 512) {
            int level = dmxData[extraChannel];
            qDebug() << "8-bit channel" << extraChannel << "level:" << level;
            // Ajoutez votre traitement pour ce canal 8 bits ici
        }
    }
}
