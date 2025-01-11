#include "mainwindow.h"
//#include <iostream>
//#include "dmxreceiver.h"

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


WindowP::WindowP() :    QWidget()

{   //initialisations
    dmxData.resize(512, 0); // Initialiser dmxData avec 512 valeurs de 0
    previousDMXData.resize(512, 0); // Initialiser previousDMXData
    setupNetworkInterfaces();

    view  = new QGraphicsView(this) ;
    scene = new QGraphicsScene(this);
    view->setScene(scene);
    showFullScreen();


    // Récupérer la définition de l'écran
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    view->setSceneRect(screenGeometry);  // Définir la taille de la vue
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    view->setFixedWidth(screenWidth);
    view->setFixedHeight(screenHeight);

    // fond noir et meilleure qualité
    view->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    view->setAutoFillBackground(true);
    view->setRenderHints(QPainter::Antialiasing);
    view->setRenderHints(QPainter::SmoothPixmapTransform);

    // Remove the border and frame
    view->setFrameShape(QFrame::NoFrame);

    // Hide the cursor
    view->setCursor(Qt::BlankCursor);

    // no scrollBars
    view->setVerticalScrollBarPolicy  ( Qt::ScrollBarAlwaysOff );
    view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );


    // Add items to the scene
    rect1 =   new QGraphicsRectItem();
    rect2 =   new QGraphicsRectItem();
    ellipse1= new QGraphicsEllipseItem();
    ellipse2= new QGraphicsEllipseItem();
    pix =     new QGraphicsPixmapItem();
    scene->addItem(rect1);
    scene->addItem(rect2);
    scene->addItem(ellipse1);
    scene->addItem(ellipse2);
    scene->addItem(pix);

}
// Override the keyPressEvent to handle the backspace key
void WindowP::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Backspace) {
        QApplication::quit();

    } else {
        QWidget::keyPressEvent(event);
    }
}
void WindowP::processPendingDatagrams() //OSC traitement
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
        if ((msg->match("/device/FOCUS/PAN").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())   &&(channel<=iarg && iarg<=(channel+4)))   {pan        (iarg, iarg2);}
        if ((msg->match("/device/FOCUS/TILT").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())  &&(channel<=iarg && iarg<=(channel+4)))   {tilt       (iarg, iarg2);}
        if ((msg->match("/device/BEAM/IRIS").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())   &&(channel<=iarg && iarg<=(channel+4)))   {largeur    (iarg, iarg2);}
        if ((msg->match("/device/BEAM/FOCUS").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())  &&(channel<=iarg && iarg<=(channel+4)))   {hauteur    (iarg, iarg2);}
        if ((msg->match("/device/BEAM/FROST").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())  &&(channel<=iarg && iarg<=(channel+4)))   {rotate     (iarg, iarg2);}
        if ((msg->match("/device/COLOUR/RED").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())  &&(channel<=iarg && iarg<=(channel+4)))   {red        (iarg, iarg2);}
        if ((msg->match("/device/COLOUR/GREEN").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())&&(channel<=iarg && iarg<=(channel+4)))   {green      (iarg, iarg2);}
        if ((msg->match("/device/COLOUR/BLUE").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs()) &&(channel<=iarg && iarg<=(channel+4)))   {blue       (iarg, iarg2);}
        if ((msg->match("/device/BEAM/ZOOM").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())   &&(channel<=iarg && iarg<=(channel+4)))   {thickness  (iarg, iarg2);}
        if ((msg->match("/device/EFFECT/GOBO").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs()) &&(iarg=channel+4))                       {picture    (iarg2);}
           }
}
}


void WindowP::masterLevel (int ch, int value)
{
    //qDebug()<<"level"<<value;
    if (value>=0) master[ch-channel] = value;
    ligneUpdate();
}

void WindowP::redSacn(int ch, int value)
{  //qDebug()<<"R "<<value;
    if (value>=0) rouge[ch-channel] = value;
    ligneUpdate();
}

void WindowP::greenSacn(int ch, int value)
{  //qDebug()<<"G "<<value;
    if (value>=0) vert[ch-channel] = value;
    ligneUpdate();
}

void WindowP::blueSacn(int ch, int value)
{  //qDebug()<<"B "<<value;
    if (value>=0) bleu[ch-channel] = value;
    ligneUpdate();
}

void WindowP::red(int ch, int value)
{  //qDebug()<<"R "<<value;
    rouge[ch-channel] = ((value+1)/256);
    ligneUpdate();
}

void WindowP::green(int ch, int value)
{  //qDebug()<<"G "<<value;
    vert[ch-channel] = ((value+1)/256);
    ligneUpdate();
}

void WindowP::blue(int ch, int value)
{  //qDebug()<<"B "<<value;
    bleu[ch-channel] = ((value+1)/256);
    ligneUpdate();
}
void WindowP::pan (int ch, int value)
{   //qDebug()<<"pan "<<value;
if (value>=0) X1[ch-channel] = value;
ligneUpdate();
}

void WindowP::tilt (int ch, int value)
{  //qDebug()<<"tilt "<<value;
if (value>=0) Y1[ch-channel] = value;
ligneUpdate();
}

void WindowP::largeur (int ch, int value)
{  //qDebug()<<"L "<<value;
if (value>=0) X2[ch-channel] = value;
ligneUpdate();
}

void WindowP::hauteur (int ch, int value)
{  //qDebug()<<"H "<<value;
if (value>=0) Y2[ch-channel] = value;
ligneUpdate();
}

void WindowP::rotate(int ch, int value)
{  //qDebug()<<"rotate "<<value;
if (value>=0) R[ch-channel] = 360*value/65535;
ligneUpdate();
}

void WindowP::thickness(int ch, int value)
{  //qDebug()<<"thick "<<value;
if (value>=0) epais[ch-channel] = value*size().width()/650000;
ligneUpdate();
}

void WindowP::picture(int value)
{
    QString path = (QCoreApplication::applicationDirPath() + "/imageLine/");
    QDir dir(path);
    if (!dir.exists()) {dir.mkpath(".");}


    switch (value)
    {
    case 0:
        pict = QPixmap();
        pix->setPixmap (pict);
        scene->addItem(pix);
        break;
    case 2944:
        pict = QPixmap();
        pix->setPixmap (pict);
        scene->addItem(pix);
        break;
    case 8960:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/1.png");
        if (pict.isNull()) break;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        scene->addItem(pix);
        break;
    case 14848:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/2.png");
        if (pict.isNull()) break;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        scene->addItem(pix);
        break;
    case 20736:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/3.png");
        if (pict.isNull()) break;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        scene->addItem(pix);
        break;
    case 26624:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/4.png");
        if (pict.isNull()) break;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        scene->addItem(pix);
        break;
    case 32512:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/5.png");
        if (pict.isNull()) break;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        scene->addItem(pix);
        break;
    case 38400:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/6.png");
        if (pict.isNull()) break;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        scene->addItem(pix);
        break;
    case 44288:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/7.png");
        if (pict.isNull()) break;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        scene->addItem(pix);
        break;
    case 50176:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/8.png");
        if (pict.isNull()) break;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        scene->addItem(pix);
        break;
    case 56064:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/9.png");
        if (pict.isNull()) break;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        scene->addItem(pix);
        break;
    case 62208:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/10.png");
        if (pict.isNull()) break;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        scene->addItem(pix);
        break;
    }
    ligneUpdate();
}

void WindowP::pictureSacn (int level) //gobos loading sACN

{
    QString path = (QCoreApplication::applicationDirPath() + "/imageLine/");
    QDir dir(path);
    if (!dir.exists()) {dir.mkpath(".");}

    // scene->removeItem(pix);
    //qDebug() <<"gobo init";

    if (level >= 0 && level < 23) {
        pict = QPixmap();
        pix->setPixmap (pict);
        //scene->addItem(pix);
        //qDebug() <<"gobo 0";

    } else if (level >= 23 && level < 46) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/1.png");
        if (pict.isNull()) return;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        //scene->addItem(pix);
        //qDebug() <<"gobo 1";

    } else if (level >= 46 && level < 69) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/2.png");
        if (pict.isNull()) return;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        //scene->addItem(pix);
        //qDebug() <<"gobo 2";

    } else if (level >= 69 && level < 92) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/3.png");
        if (pict.isNull()) return;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        //scene->addItem(pix);

    } else if (level >= 92 && level < 115) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/4.png");
        if (pict.isNull()) return;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        //scene->addItem(pix);

    } else if (level >= 115 && level < 138) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/5.png");
        if (pict.isNull()) return;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        //scene->addItem(pix);

    } else if (level >= 138 && level < 161) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/6.png");
        if (pict.isNull()) return;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        //scene->addItem(pix);

    } else if (level >= 161 && level < 184) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/7.png");
        if (pict.isNull()) return;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        //scene->addItem(pix);

    } else if (level >= 184 && level < 207) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/8.png");
        if (pict.isNull()) return;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        //scene->addItem(pix);

    } else if (level >= 207 && level < 230) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/9.png");
        if (pict.isNull()) return;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        //scene->addItem(pix);

    } else if (level >= 230 && level <= 255) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/10.png");
        if (pict.isNull()) return;
        pict =  pict.scaled(size().width(), size().height());
        pix->setPixmap (pict);
        //scene->addItem(pix);

    } else {
        qDebug() << "Valeur de niveau hors de la plage attendue";
    }
}


void WindowP::ligneUpdate() //drawings
{
    rect1->setRect(QRectF((X1[0]*size().width()/65535), (Y1[0]*size().height()/65535), (X2[0]*size().width()/65535), (Y2[0]*size().height()/65535)));
    rect1->setPen(QPen(QColor(rouge[0], vert[0], bleu[0], master[0]), epais[0], Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    rect1->setTransformOriginPoint(rect1->boundingRect().center());
    rect1->setRotation(R[0]);

    rect2->setRect(QRectF(((size().width()-X1[1]*size().width()/65535)-X2[1]*size().width()/65535), ((size().height()-Y1[1]*size().height()/65535)-Y2[1]*size().height()/65535), (X2[1]*size().width()/65535), (Y2[1]*size().height()/65535)));
    rect2->setPen(QPen(QColor(rouge[1], vert[1], bleu[1], master[1]), epais[1],Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    rect2->setTransformOriginPoint(rect2->boundingRect().center());
    rect2->setRotation(R[1]);

    ellipse1->setRect(QRectF((X1[2]*size().width()/65535), 2*((size().height()-Y1[2]*size().height()/65535)-Y2[2]*size().height()/65535), (2*X2[2]*size().width()/65535), (2*Y2[2]*size().height()/65535)));
    ellipse1->setPen(QPen(QColor(rouge[2], vert[2], bleu[2], master[2]), epais[2], Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    ellipse1->setTransformOriginPoint(ellipse1->boundingRect().center());
    ellipse1->setRotation(R[2]);

    ellipse2->setRect(QRectF((X1[3]*size().width()/65535), ((size().height()-Y1[3]*size().height()/65535)-Y2[3]*size().height()/65535), (X2[3]*size().width()/65535), (Y2[3]*size().height()/65535)));
    ellipse2->setPen(QPen(QColor(rouge[3], vert[3], bleu[3], master[3]), epais[3],Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    ellipse2->setTransformOriginPoint(ellipse2->boundingRect().center());
    ellipse2->setRotation(R[3]);

    pix->setTransformationMode(Qt::SmoothTransformation);
    float xPix = X1[4]*size().width();
    float yPix = Y1[4]*size().height();
    float hPix = X2[4]*size().width()/127;
    float vPix = Y2[4]*size().height()/127;
    pix->setOpacity(master[4]/255);
    QTransform trans;
    trans.setMatrix(hPix/65535,0,0,0,vPix/65535,0,xPix/65535,yPix/65535,1);
    pix->setTransform(trans);
    pix->setTransformOriginPoint(pix->boundingRect().center());
    pix->setRotation(R[4]);
}

void WindowP::setupNetworkInterfaces() { //interface sACN et OSC
    //OSC
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(7003);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

}

void WindowP::updateDMXData(const std::vector<int>& newData) {
    dmxData.clear();
    for (size_t i = 0; i < newData.size(); ++i) {
        dmxData.push_back(newData[i]); // Copier les valeurs sans modification
    }

    // Log the values to check if they are correctly copied
   /* std::cout << "Valeurs copiées dans updateDMXData: ";
    for (const auto& val : dmxData) {
        std::cout << val << " ";
    }
    std::cout << std::endl;*/

    processDMXData(); // Traiter les nouvelles données après la mise à jour

    // Mettre à jour previousDMXData avec les nouvelles valeurs
    previousDMXData = dmxData;
}
void WindowP::processDMXData() {
    // Affichage des valeurs
   /* for (int i = 0; i < dmxData.size(); ++i) {
        std::cout << "Channel " << i << " Value: " << dmxData[i] << std::endl; // Afficher le numéro de canal et la valeur
    }*/

    // Traiter les données DMX pour les canaux entre 100 et 199
    for (int baseChannel = 0; baseChannel < 100; baseChannel += 20) {
        // Définir les valeurs de `ch` en fonction de baseChannel
        int ch = 0;
        switch (baseChannel) {
        case 0: ch = 61; break;
        case 20: ch = 62; break;
        case 40: ch = 63; break;
        case 60: ch = 64; break;
        case 80: ch = 65; break;
        }

        // Traiter les canaux 8 bits (100-103, 120-123, 140-143, etc.)
        for (int i = 0; i < 4; ++i) {
            int channel = baseChannel + i;
            if (channel < 97 && channel>=0) {
                int level = dmxData[channel];
                if (level != previousDMXData[channel]) { // Traiter seulement si la valeur a changé
                // traitement pour les 4 premiers canaux 8 bits
                //qDebug() << "ch " << channel << "level " << level;
                if (!(level < 0)) switch (i) {
                    case 0: masterLevel(ch, level); break;
                    case 1: redSacn(ch, level); break;
                    case 2: greenSacn(ch, level); break;
                    case 3: blueSacn(ch, level); break;
                    }
            }}
        }

        // Traiter les canaux 16 bits (104-116, 124-136, 144-156, etc.)
        for (int i = 4; i < 16; i += 2) {
            int highChannel = baseChannel + i;
            int lowChannel = baseChannel + i + 1;
            if (highChannel < 512 && lowChannel < 512) {
                int highByte = dmxData[highChannel];
                int lowByte = dmxData[lowChannel];
                int combinedValue = (highByte << 8) | lowByte;
                int previousCombinedValue = (previousDMXData[highChannel] << 8) | previousDMXData[lowChannel];
                if (combinedValue != previousCombinedValue) { // Traiter seulement si la valeur a changé
                if (!(highByte < 0)&&!(lowByte < 0))switch (i) {
                    case 4: pan(ch, combinedValue); break;
                    case 6: tilt(ch, combinedValue); break;
                    case 8: largeur(ch, combinedValue); break;
                    case 10: hauteur(ch, combinedValue); break;
                    case 12: thickness(ch, combinedValue); break;
                    case 14: rotate(ch, combinedValue); break;
                    }}
            }
        }

        int level = dmxData[96];
        //qDebug() << "8-bit channel" << "196" << "level:" << level;
        // traitement pour ce canal 8 bits ici
        if (!(level < 0)) pictureSacn(level);

    }
}
