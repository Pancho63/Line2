// Copyright 2016 Tom Barthel-Steer
// http://www.tomsteer.net
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SACNSOCKET_H
#define SACNSOCKET_H

#include <QObject>
#include <QUdpSocket>
#include <QVariant>

class sACNRxSocket : public QUdpSocket
{
    Q_OBJECT
public:
    explicit sACNRxSocket(QObject *parent = Q_NULLPTR);

    bool bindMulticast(quint16 universe);
    bool bindUnicast();

    /**
     * @brief setNetworkInterface sets the network interface used to receive sACN data
     * Must be called before any other library function to take effect!
     * @param iface a network interface
     */
    static void setNetworkInterface(QNetworkInterface iface);

private:
    static QNetworkInterface s_networkInterace;
};

class sACNTxSocket : public QUdpSocket
{
    Q_OBJECT
public:
    explicit sACNTxSocket(QObject *parent = Q_NULLPTR);

    bool bindMulticast();

    /**
     * @brief setNetworkInterface sets the network interface used to transmit sACN data
     * Must be called before any other library function to take effect!
     * @param iface a network interface
     */
    static void setNetworkInterface(QNetworkInterface iface);

private:
    static QNetworkInterface s_networkInterace;
};


#endif // SACNSOCKET_H
