//
// Created by alex on 07/12/19.
//
/*BENVENUTO nel codice sorgente della classe server_virgilium
 * questa classe è quella in cui si concentrano tutte le rogne legate
 * ai socket.
 * il membro this->server è un socket che in fase di costruzione viene connesso
 * al server.
 * il membro this->in è uno stream in cui vengono scritti i dati in ingresso e da cui
 * bisogna leggere.
 * Ti consiglio di guardare il metodo readyRead() perché è quello in cui ho inventato una
 * sorta di protocollo basato su un codice intero di tipo _int.
 * Ti consiglio di guardare il file constants.h perché contiene le define dei codici.
 * Ogni codice indica una azione diversa che deve essere intrapresa dal client e dal server.
 *
 *
 *
 * */
#ifndef VIRGILIUMCLIENT_SERVER_VIRGILIUM_H
#define VIRGILIUMCLIENT_SERVER_VIRGILIUM_H
#include <QtNetwork>
#include <QObject>
#include <queue>
#include "../common/constants.h"
#include "../common/messages/CrdtMessage.h"



class CrdtMessage;
class virgilium_client;

class server_virgilium: public QObject {
    std::queue<CrdtMessage> messages;
    virgilium_client& client;
    QTcpSocket *server;
    //QDataStream in;
public:
    server_virgilium(QObject *parent,QString ServerAddress,_int serverPort,virgilium_client& client);
    /*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
     * il costruttore istanza il socket nello heap(devo vedere se va bene nello heap).
     * poi effettua il bind tra i segnali che arrivano da suo papà (il client) e i suoi slot.
     * Dopodichè contatta il server all'indirizzo hard-coded(per ora) e si fa dare il site-id.
     * TI consiglio di guardare il metodo ReadyRead e il file constants.h
     * */
    void doConnect();
    ~server_virgilium();

    void send(CrdtMessage& m,_int code);
    void dispatchMessages();
    /*void mandaqualcosa();*/
    signals:
public slots:
    void connected();
    void disconnected();
    void bytesWritten(_int bytes);
    void readyRead();
    void site_id_assignment();
};


#endif //VIRGILIUMCLIENT_SERVER_VIRGILIUM_H
