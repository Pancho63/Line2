#ifndef DMXRECEIVER_H
#define DMXRECEIVER_H

#include <QObject>
#include <curl/curl.h>
#include <include/json.hpp>
#include <mainwindow.h>


class DMXReceiver : public QObject {
    Q_OBJECT

public:
    DMXReceiver(QObject *parent = nullptr, WindowP *window = nullptr); // Update this line
    ~DMXReceiver();

public slots:
    void updateDMX();

private:
    CURL* curl;
    std::vector<int> dmxData; // Tableau pour stocker les données DMX
     WindowP* windowPInstance;
};

#endif // DMXRECEIVER_H
