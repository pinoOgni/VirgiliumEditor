//
// Created by alex on 17/12/19.
//

#ifndef VIRGILIUMSERVER_VIRGILIUM_CLIENT_H
#define VIRGILIUMSERVER_VIRGILIUM_CLIENT_H
#include <QObject>
#include <QtNetwork>
#include "constants.h"
#include "virgilium_server.h"

class virgilium_client: public QObject {
    virgilium_server* server;
    _int siteId;
    QTcpSocket *socket;
   // QDataStream in;

public:
    virgilium_client(virgilium_server* server,QTcpSocket *socket);
    void setSiteId(_int);
    _int getSiteId();

    void sendMessage(message& m,_int code);

    ~virgilium_client();
public slots:
    void readyRead();
    void connected();
    void disconnected();
    void bytesWritten(_int bytes);
    void site_id_assignment();
};


#endif //VIRGILIUMSERVER_VIRGILIUM_CLIENT_H
