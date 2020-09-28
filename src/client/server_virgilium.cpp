//
// Created by alex on 07/12/19.
//

#include <iostream>
#include "server_virgilium.h"
#include "virgilium_client.h"

server_virgilium::server_virgilium(QObject *parent, QString serverAddress, _int serverPort, virgilium_client &client)
        : QObject(parent), client(client) {
    this->server = new QTcpSocket(this);
    connect(this->server, &QTcpSocket::connected, this, &server_virgilium::connected);
    connect(this->server, &QTcpSocket::disconnected, this, &server_virgilium::disconnected);
    connect(this->server, &QTcpSocket::bytesWritten, this, &server_virgilium::bytesWritten);
    connect(this->server, &QTcpSocket::readyRead, this, &server_virgilium::readyRead);

    /*this->in.setDevice(this->server); // OCCHIO
    this->in.setVersion(Q_DATA_STREAM_VERSION);*/

    this->server->connectToHost(serverAddress, serverPort);
    if (!this->server->waitForConnected(ConnectionWaitingTime)) {
        qDebug() << "Error: " << this->server->errorString();
    }
}


void server_virgilium::connected() {
    qDebug() << "Connection to server ok!\n";
    return;
}

void server_virgilium::disconnected() {
    qDebug() << "Disconnected from server...\n";
    return;
}

void server_virgilium::bytesWritten(_int bytes) {
    qDebug() << bytes << " written";
    return;
}

void server_virgilium::readyRead() {
    /*  this->in.startTransaction(); questa istruzione per assicurarsi che siano arrivati tutti i pacchetti
     *   if(!in.commitTransaction()) questa se qualcosa non funonzia
     *
     *
     * */
    /*
     * QUESTO METODO E' FONDAMENTALE, ESISTE IL SUO DUALE NEL SERVER.
     * LA PRIMA COSA CHE FA È LEGGERE L'INTERO code DALLO STREAM ( UN INTERO DI LUNGHEZZA PREFISSATA!!!)
     * IN BASE AL VALORE DI QUESTO INTERO (I CUI VALORI SONO DEFINITI IN constants.h FA COSE DIVERSE
     * NELLO SWITCH CI SONO I VARI CASE, AD OGNI CASE EMETTE UN SEGNALE DIVERSO (PER OGNI SEGNALE C'E' UNO SLOT)
     * (PER OGNI SU C'È SEMPRE UN GIÙ PER OGNI MEN C'È SEMPRE PIÙ, QUESTO IL MONDO FA GIRAR)*/

    /*_int code;*/
    qDebug() << "e' arrivato qualcosa dal server..\n";
    QDataStream in(this->server);
    in.setVersion(Q_DATA_STREAM_VERSION);

        while(1) {
            _int code;
            in.startTransaction();
            in >> code;

            if(code == 0)
                break;
            if (in.commitTransaction()) {
                switch (code) {
                    case SITE_ID_ASSIGNMENT: {
                        qDebug() << "MI STANNO ASSEGNANDO UN SITE_ID IHIHIH ICSDI\n";
                        emit(this->client.site_id_assignment());
                        break;
                    }
                    case SYMBOL_INSERT_OR_ERASE: {
                        CrdtMessage m = CrdtMessage();

                        in.startTransaction();
                        in >> m;
                        if (!in.commitTransaction()) {
                            return;
                        }

                        this->client.process(m);
                        break;
                    }
                    /* Aggiunto per cursore */
                    case CURSOR_CHANGED: {
                        CrdtMessage m = CrdtMessage();

                        in.startTransaction();
                        in >> m;
                        if (!in.commitTransaction()) {
                            return;
                        }

                        this->client.changeCursorPosition(m);
                        break;
                    }
                }
            } else {
                return;
            }
        }

    //TODO
}

server_virgilium::~server_virgilium() {
    this->server->disconnect();
    delete (this->server);
}


void server_virgilium::send(CrdtMessage &m, _int code) {
    //TODO
    /*Qua devo preparare una roba da mandare al server*/
    QByteArray blocco;
    QDataStream out(&blocco, QIODevice::ReadWrite);
    out.setVersion(Q_DATA_STREAM_VERSION);

    out << code;
    out << m;
    //this->server->startTransaction();
    this->server->write(blocco);
//     if(this->server.commitTransaction()){
//
//     }


    return;
}

void server_virgilium::dispatchMessages() {
    //TODO

    return;
}

void server_virgilium::site_id_assignment() {
    _int siteId;
    //qDebug()<<"Mi stanno per assegnare il site id ihihihihih icsdì\n";
    QDataStream in(this->server);
    in.setVersion(Q_DATA_STREAM_VERSION);

        in.startTransaction();
        in >> siteId; //leggo il mio site_id dal socket
        this->client.set_site_id(siteId);
        if (in.commitTransaction()){
            return;
        } else {
            this->client.set_site_id(siteId);
            return;
        }
}

/*
void server_virgilium::mandaqualcosa() {

    qDebug("Sto mandandoooo");
    _int prova = SYMBOL_INSERT_OR_ERASE;
    QVector<_int> provaprova;
    provaprova.push_back(121);
    provaprova.push_back(23);
    provaprova.push_back(3412);

    message provames(symbol("c", 1, 1, provaprova), 1, "INSERT");


    QByteArray blocco;
    QDataStream out(&blocco, QIODevice::ReadWrite);
    out.setVersion(Q_DATA_STREAM_VERSION);
    out << prova;

    out << provames;
    this->server->write(blocco);


}*/
