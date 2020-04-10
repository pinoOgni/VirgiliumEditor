//
// Created by alex on 17/12/19.
//

#ifndef VIRGILIUMSERVER_VIRGILIUM_SERVER_H
#define VIRGILIUMSERVER_VIRGILIUM_SERVER_H
#include "constants.h"
#include <vector>
#include <queue>
#include <QObject>
#include <algorithm>
#include <QtNetwork>
#include "message.h"

class virgilium_client;

class symbol;

class virgilium_server: public QObject {
    Q_OBJECT
    QTcpServer server;
    std::map<_int,virgilium_client*> editors;
    std::queue<message> messages;
    _int IDs;
public:
    explicit virgilium_server(QObject *parent);
    _int connect_to_client(virgilium_client* virgilium_client);
    /*
     * Registra, nel proprio vettore di puntatori a virgilium_client, il puntatore ricevuto e restituisce un
        identificatore univoco mediante il quale l’editor potrà distinguersi dalle altre istanze della stessa
        classe
     */
    void disconnect(virgilium_client* virgilium_client);
    //Elimina il puntatore dal vettore degli editor collegati.
    void send(const message& m);
    // Aggiunge il messaggio m alla coda dei messaggi da distribuire, senza ancora mandarlo.

    void dispatchmessages();
    /*Distribuisce tutti i messaggi accodati a tutti gli editor attualmente collegati, fatta eccezione, però,
            per l’originatore del messaggio stesso.
    */
    signals:

public slots:
    void newConnection();

};




#endif //VIRGILIUMSERVER_VIRGILIUM_SERVER_H
