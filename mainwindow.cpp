#include "mainwindow.h"
#include <QScreen>
#include <fstream>
#include <sstream>

float getCPUUsage()
{
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);
    std::istringstream ss(line);
    std::string cpu;
    float user, nice, system, idle;
    ss >> cpu >> user >> nice >> system >> idle;
    return (user + nice + system) / (user + nice + system + idle) * 100;
}

int iarg;
int iarg2;
std::string sarg;
qreal master[5] = {0, 0, 0, 0, 0};
qreal X1[5] = {0, 0, 0, 0, 0};
qreal X2[5] = {0, 0, 0, 0, 0};
qreal Y1[5] = {0, 0, 0, 0, 0};
qreal Y2[5] = {0, 0, 0, 0, 0};
qreal R[5] = {0, 0, 0, 0, 0};
qreal rouge[5] = {0, 0, 0, 0, 0};
qreal vert[5] = {0, 0, 0, 0, 0};
qreal bleu[5] = {0, 0, 0, 0, 0};
qreal epais[5] = {0, 0, 0, 0, 0};
int channel = 61;

QSharedPointer<sACNListener> listener;



WindowP::WindowP()
    : QWidget()
    , dmxData(71, 0)

{ //initialisations

    setupNetworkInterfaces();

    view = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    view->setScene(scene);
    showFullScreen();

    // Récupérer la définition de l'écran
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    view->setSceneRect(screenGeometry); // Définir la taille de la scène
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    // y adapter la vue
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
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Add items to the scene
    rect1 = new QGraphicsRectItem();
    rect2 = new QGraphicsRectItem();
    ellipse1 = new QGraphicsEllipseItem();
    ellipse2 = new QGraphicsEllipseItem();
    pix = new QGraphicsPixmapItem();
    scene->addItem(rect1);
    scene->addItem(rect2);
    scene->addItem(ellipse1);
    scene->addItem(ellipse2);
    scene->addItem(pix);
}
// Override the keyPressEvent to handle the backspace key
void WindowP::keyPressEvent(QKeyEvent *event)
{
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
            if ((msg->match("/circ/level").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())
                && (channel <= iarg && iarg <= (channel + 4))) {
                masterLevel(iarg, iarg2);
            }
            if ((msg->match("/device/FOCUS/PAN").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())
                && (channel <= iarg && iarg <= (channel + 4))) {
                pan(iarg, iarg2);
            }
            if ((msg->match("/device/FOCUS/TILT").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())
                && (channel <= iarg && iarg <= (channel + 4))) {
                tilt(iarg, iarg2);
            }
            if ((msg->match("/device/BEAM/IRIS").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())
                && (channel <= iarg && iarg <= (channel + 4))) {
                largeur(iarg, iarg2);
            }
            if ((msg->match("/device/BEAM/FOCUS").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())
                && (channel <= iarg && iarg <= (channel + 4))) {
                hauteur(iarg, iarg2);
            }
            if ((msg->match("/device/BEAM/FROST").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())
                && (channel <= iarg && iarg <= (channel + 4))) {
                rotate(iarg, iarg2);
            }
            if ((msg->match("/device/COLOUR/RED").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())
                && (channel <= iarg && iarg <= (channel + 4))) {
                red(iarg, iarg2);
            }
            if ((msg->match("/device/COLOUR/GREEN").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())
                && (channel <= iarg && iarg <= (channel + 4))) {
                green(iarg, iarg2);
            }
            if ((msg->match("/device/COLOUR/BLUE").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())
                && (channel <= iarg && iarg <= (channel + 4))) {
                blue(iarg, iarg2);
            }
            if ((msg->match("/device/BEAM/ZOOM").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())
                && (channel <= iarg && iarg <= (channel + 4))) {
                thickness(iarg, iarg2);
            }
            if ((msg->match("/device/EFFECT/GOBO").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())
                && (iarg = channel + 4)) {
                picture(iarg2);
            }
        }
    }
}

void WindowP::masterLevel(int ch, int value)
{
    //qDebug()<<"level"<<value;
    if (value >= 0)
        master[ch - channel] = value;
    ligneUpdate();
}

void WindowP::redSacn(int ch, int value)
{ //qDebug()<<"R "<<value;
    if (value >= 0)
        rouge[ch - channel] = value;
    ligneUpdate();
}

void WindowP::greenSacn(int ch, int value)
{ //qDebug()<<"G "<<value;
    if (value >= 0)
        vert[ch - channel] = value;
    ligneUpdate();
}

void WindowP::blueSacn(int ch, int value)
{ //qDebug()<<"B "<<value;
    if (value >= 0)
        bleu[ch - channel] = value;
    ligneUpdate();
}

void WindowP::red(int ch, int value)
{ //qDebug()<<"R "<<value;
    rouge[ch - channel] = ((value + 1) / 256);
    ligneUpdate();
}

void WindowP::green(int ch, int value)
{ //qDebug()<<"G "<<value;
    vert[ch - channel] = ((value + 1) / 256);
    ligneUpdate();
}

void WindowP::blue(int ch, int value)
{ //qDebug()<<"B "<<value;
    bleu[ch - channel] = ((value + 1) / 256);
    ligneUpdate();
}
void WindowP::pan(int ch, int value)
{ //qDebug()<<"pan "<<value;
    if (value >= 0)
        X1[ch - channel] = value;
    ligneUpdate();
}

void WindowP::tilt(int ch, int value)
{ //qDebug()<<"tilt "<<value;
    if (value >= 0)
        Y1[ch - channel] = value;
    ligneUpdate();
}

void WindowP::largeur(int ch, int value)
{ //qDebug()<<"L "<<value;
    if (value >= 0)
        X2[ch - channel] = value;
    ligneUpdate();
}

void WindowP::hauteur(int ch, int value)
{ //qDebug()<<"H "<<value;
    if (value >= 0)
        Y2[ch - channel] = value;
    ligneUpdate();
}

void WindowP::rotate(int ch, int value)
{ //qDebug()<<"rotate "<<value;
    if (value >= 0)
        R[ch - channel] = 360 * value / 65535;
    ligneUpdate();
}

void WindowP::thickness(int ch, int value)
{ //qDebug()<<"thick "<<value;
    if (value >= 0)
        epais[ch - channel] = value * size().width() / 650000;
    ligneUpdate();
}

void WindowP::picture(int value)
{
    QString path = (QCoreApplication::applicationDirPath() + "/imageLine/");
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    //scene->removeItem(pix);

    switch (value) {
    case 0:
        pict = QPixmap();
        pix->setPixmap(pict);
        scene->addItem(pix);
        break;
    case 2944:
        pict = QPixmap();
        pix->setPixmap(pict);
        scene->addItem(pix);
        break;
    case 8960:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/1.png");
        if (pict.isNull())
            break;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        scene->addItem(pix);
        break;
    case 14848:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/2.png");
        if (pict.isNull())
            break;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        scene->addItem(pix);
        break;
    case 20736:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/3.png");
        if (pict.isNull())
            break;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        scene->addItem(pix);
        break;
    case 26624:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/4.png");
        if (pict.isNull())
            break;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        scene->addItem(pix);
        break;
    case 32512:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/5.png");
        if (pict.isNull())
            break;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        scene->addItem(pix);
        break;
    case 38400:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/6.png");
        if (pict.isNull())
            break;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        scene->addItem(pix);
        break;
    case 44288:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/7.png");
        if (pict.isNull())
            break;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        scene->addItem(pix);
        break;
    case 50176:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/8.png");
        if (pict.isNull())
            break;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        scene->addItem(pix);
        break;
    case 56064:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/9.png");
        if (pict.isNull())
            break;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        scene->addItem(pix);
        break;
    case 62208:
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/10.png");
        if (pict.isNull())
            break;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        scene->addItem(pix);
        break;
    }
    ligneUpdate();
}

void WindowP::pictureSacn(int level) //gobos loading sACN

{
    QString path = (QCoreApplication::applicationDirPath() + "/imageLine/");
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // scene->removeItem(pix);
    //qDebug() <<"gobo init";

    if (level >= 0 && level < 23) {
        pict = QPixmap();
        pix->setPixmap(pict);
        //scene->addItem(pix);
        //qDebug() <<"gobo 0";

    } else if (level >= 23 && level < 46) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/1.png");
        if (pict.isNull()) {
            qDebug() << "gobo 1 null "
                     << (QCoreApplication::applicationDirPath() + "/imageLine/1.png");
            return;
        }
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        //scene->addItem(pix);
        //qDebug() <<"gobo 1";

    } else if (level >= 46 && level < 69) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/2.png");
        if (pict.isNull())
            return;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        //scene->addItem(pix);
        //qDebug() <<"gobo 2";

    } else if (level >= 69 && level < 92) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/3.png");
        if (pict.isNull())
            return;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        //scene->addItem(pix);

    } else if (level >= 92 && level < 115) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/4.png");
        if (pict.isNull())
            return;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        //scene->addItem(pix);

    } else if (level >= 115 && level < 138) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/5.png");
        if (pict.isNull())
            return;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        //scene->addItem(pix);

    } else if (level >= 138 && level < 161) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/6.png");
        if (pict.isNull())
            return;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        //scene->addItem(pix);

    } else if (level >= 161 && level < 184) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/7.png");
        if (pict.isNull())
            return;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        //scene->addItem(pix);

    } else if (level >= 184 && level < 207) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/8.png");
        if (pict.isNull())
            return;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        //scene->addItem(pix);

    } else if (level >= 207 && level < 230) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/9.png");
        if (pict.isNull())
            return;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        //scene->addItem(pix);

    } else if (level >= 230 && level <= 255) {
        pict = QPixmap(QCoreApplication::applicationDirPath() + "/imageLine/10.png");
        if (pict.isNull())
            return;
        pict = pict.scaled(size().width(), size().height());
        pix->setPixmap(pict);
        //scene->addItem(pix);

    } else {
        qDebug() << "Valeur de niveau hors de la plage attendue";
    }
}

void WindowP::ligneUpdate() //drawings
{
    rect1->setRect(QRectF((X1[0] * size().width() / 65535),
                          (Y1[0] * size().height() / 65535),
                          (X2[0] * size().width() / 65535),
                          (Y2[0] * size().height() / 65535)));
    rect1->setPen(QPen(QColor(rouge[0], vert[0], bleu[0], master[0]),
                       epais[0],
                       Qt::SolidLine,
                       Qt::RoundCap,
                       Qt::RoundJoin));
    rect1->setTransformOriginPoint(rect1->boundingRect().center());
    rect1->setRotation(R[0]);

    rect2->setRect(
        QRectF(((size().width() - X1[1] * size().width() / 65535) - X2[1] * size().width() / 65535),
               ((size().height() - Y1[1] * size().height() / 65535)
                - Y2[1] * size().height() / 65535),
               (X2[1] * size().width() / 65535),
               (Y2[1] * size().height() / 65535)));
    rect2->setPen(QPen(QColor(rouge[1], vert[1], bleu[1], master[1]),
                       epais[1],
                       Qt::SolidLine,
                       Qt::RoundCap,
                       Qt::RoundJoin));
    rect2->setTransformOriginPoint(rect2->boundingRect().center());
    rect2->setRotation(R[1]);

    ellipse1->setRect(QRectF((X1[2] * size().width() / 65535),
                             ((size().height() - Y1[2] * size().height() / 65535)
                              - Y2[2] * size().height() / 65535),
                             (2 * X2[2] * size().width() / 65535),
                             (2 * Y2[2] * size().height() / 65535)));
    ellipse1->setPen(QPen(QColor(rouge[2], vert[2], bleu[2], master[2]),
                          epais[2],
                          Qt::SolidLine,
                          Qt::RoundCap,
                          Qt::RoundJoin));
    ellipse1->setTransformOriginPoint(ellipse1->boundingRect().center());
    ellipse1->setRotation(R[2]);

    ellipse2->setRect(QRectF((X1[3] * size().width() / 65535),
                             ((size().height() - Y1[3] * size().height() / 65535)
                              - Y2[3] * size().height() / 65535),
                             (X2[3] * size().width() / 65535),
                             (Y2[3] * size().height() / 65535)));
    ellipse2->setPen(QPen(QColor(rouge[3], vert[3], bleu[3], master[3]),
                          epais[3],
                          Qt::SolidLine,
                          Qt::RoundCap,
                          Qt::RoundJoin));
    ellipse2->setTransformOriginPoint(ellipse2->boundingRect().center());
    ellipse2->setRotation(R[3]);

    pix->setTransformationMode(Qt::SmoothTransformation);
    float xPix = X1[4] * size().width();
    float yPix = Y1[4] * size().height();
    float hPix = X2[4] * size().width() / 127;
    float vPix = Y2[4] * size().height() / 127;
    pix->setOpacity(master[4] / 255);
    QTransform trans;
    trans.setMatrix(hPix / 65535, 0, 0, 0, vPix / 65535, 0, xPix / 65535, yPix / 65535, 1);
    pix->setTransform(trans);
    pix->setTransformOriginPoint(pix->boundingRect().center());
    pix->setRotation(R[4]);
}

void WindowP::setupNetworkInterfaces()
{ //interface sACN et OSC
    //OSC
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(7003);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    // sACN : Obtenir la liste de toutes les interfaces réseau
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    // Spécifiez le nom de l'interface réseau que vous souhaitez utiliser
    // Actions spécifiques en fonction du système d'exploitation

#if defined(Q_OS_MACOS)
    qDebug() << "Running on macOS";
    // Code spécifique à macOS
    QString interfaceName = "en0";

#elif defined(Q_OS_LINUX)
#if defined(__UBUNTU__)
    qDebug() << "Running on Ubuntu";
    // Code spécifique à Ubuntu
    //QString interfaceName = "enp2s0f1";
    QString interfaceName = "wlp3s0";
#elif defined(__RASPIAN__)
    qDebug() << "Running on Raspbian";
    // Code spécifique à Raspbian
    QString interfaceName = "eth0";
#else
    qDebug() << "Running on an unknown Linux distribution";
    // Code pour une distribution Linux inconnue
    QString interfaceName = "eth0";
#endif
#elif defined(Q_OS_WIN)
    qDebug() << "Running on Windows";
    // Code spécifique à Windows
    //QString interfaceName = "";
#else
    qDebug() << "Running on an unsupported OS";
    // Code pour un système d'exploitation non supporté
    //QString interfaceName = "";
#endif

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
        int universe = 7; // universe in range 1-63999
        listener = sACNManager::getInstance()->getListener(universe);
        if (listener) {
            connect(listener.data(), SIGNAL(levelsChanged()), this, SLOT(onLevelsChanged()));
            qDebug() << "Listener created for universe" << universe;
        } else {
            qDebug() << "Failed to create listener for universe" << universe;
        }
    } else {
        qDebug() << "Interface non trouvée.";
        // Iterate through the list of interfaces and print their details
        for (const QNetworkInterface &interface : interfaces) {
            qDebug() << "Interface Name:" << interface.humanReadableName();
            qDebug() << "Hardware Address:" << interface.hardwareAddress();

            // Get all IP addresses associated with this interface
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            for (const QNetworkAddressEntry &entry : entries) {
                qDebug() << "IP Address:" << entry.ip().toString();
                qDebug() << "Netmask:" << entry.netmask().toString();
                qDebug() << "Broadcast:" << entry.broadcast().toString();
            }
            qDebug() << "--------------------------------------";
        }
    }
}

void WindowP::onLevelsChanged()
{
    float cpuUsage = getCPUUsage();
    if (cpuUsage < 90.0) {
        for (int channel = 99; channel < 170; ++channel) {
            if (channel - 99 < dmxData.size() && channel < listener->mergedLevels().size()) {
                dmxData[channel - 99] = listener->mergedLevels()[channel].level;
            } else {
                qWarning() << "Channel index out of bounds:" << channel;
            }
        }
        processDMXData();
    } else {
        qWarning() << "CPU usage is too high: " << cpuUsage << "%";
    }
}

void WindowP::processDMXData()
{
    std::map<int, int> baseChannelToCh = {{0, 61}, {14, 62}, {28, 63}, {42, 64}, {56, 65}};

    for (auto &[baseChannel, ch] : baseChannelToCh) {
        int *ptr = &dmxData[baseChannel];
        for (int i = 0; i < 6; ++i, ++ptr) {
            if (baseChannel + i < dmxData.size() && *ptr >= 0) {
                switch (i) {
                case 0:
                    masterLevel(ch, *ptr);
                    break;
                case 1:
                    redSacn(ch, *ptr);
                    break;
                case 2:
                    greenSacn(ch, *ptr);
                    break;
                case 3:
                    blueSacn(ch, *ptr);
                    break;
                case 4:
                    thickness(ch, *ptr * 256);
                    break;
                case 5:
                    rotate(ch, *ptr * 256);
                    break;
                }
            }
        }

        ptr = &dmxData[baseChannel + 6];
        for (int i = 6; i < 14; i += 2, ptr += 2) {
            if (baseChannel + i < dmxData.size() && baseChannel + i + 1 < dmxData.size()
                && *ptr >= 0 && *(ptr + 1) >= 0) {
                int combinedValue = (*ptr << 8) | *(ptr + 1);
                switch (i) {
                case 6:
                    pan(ch, combinedValue);
                    break;
                case 8:
                    tilt(ch, combinedValue);
                    break;
                case 10:
                    largeur(ch, combinedValue);
                    break;
                case 12:
                    hauteur(ch, combinedValue);
                    break;
                }
            }
        }
    }

    int level = dmxData[70];
    if (level >= 0)
        pictureSacn(level);
}
