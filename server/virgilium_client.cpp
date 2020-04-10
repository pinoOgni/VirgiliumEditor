//
// Created by alex on 17/12/19.
//

#include <iostream>
#include "virgilium_client.h"
#include "message.h"

virgilium_client::virgilium_client(virgilium_server* server,QTcpSocket *socket):server(server), socket(socket){

    connect(this->socket, &QTcpSocket::connected ,  this , &virgilium_client::connected);
    connect(this->socket, &QTcpSocket::disconnected, this , &virgilium_client::disconnected);
    connect(this->socket, &QTcpSocket::bytesWritten,this, &virgilium_client::bytesWritten);
    connect(this->socket, &QTcpSocket::readyRead,this, &virgilium_client::readyRead);
    // this->in.setDevice(socket);

}

virgilium_client::~virgilium_client(){
    delete this->socket;
}


void virgilium_client::setSiteId(_int siteId) {
    this->siteId=siteId;
    return;
}

_int virgilium_client::getSiteId() {
    return this->siteId;
}

void virgilium_client::readyRead() {
    //TODO
    std::cout<<"E' arrivato qualcosa dal client con id: " << this->siteId<<std::endl;
    _int code;
    QDataStream in;
    in.setDevice(this->socket);
    in.setVersion(Q_DATA_STREAM_VERSION);
    for(;;){
        in.startTransaction();
        in>>code;
        if(in.commitTransaction()){


            switch(code) {
                case SYMBOL_INSERT_OR_ERASE:

                    message m = message();

                    for (;;){
                        in.startTransaction();
                        in >> m;
                        if(in.commitTransaction())
                            break;
                       }
                    qDebug("Arrivato insert o delete bele bele: m: ");
                    m.printMessage();
                    this->server->send(m);
                    break;
            }

        }
        else{
            qDebug("non ho più niente da leggere sul socket");
            break;
        }
        // this->server->dispatchmessages();//tentativo
    }
    this->server->dispatchmessages();
}

void virgilium_client::sendMessage(message &m, _int code) {
    std::cout<<"Sto mandando al client: "<<this->siteId<<std::endl<<"questo messaggio:";
    m.printMessage();

    QByteArray blocco;
    QDataStream out(&blocco,QIODevice::ReadWrite);
    //out.setDevice(this->socket);
    //out.setVersion(Q_DATA_STREAM_VERSION);

    out<<code;//scrivi sul socket code
    out<<m;//scrivi sul socket m (oggetto di tipo messaggio)
   // std::cout<<this->connected();
    this->socket->write(blocco);
    return;
}

void virgilium_client::connected() {
    qDebug()<< "Connection to server ok!\n";
    return;

}

void virgilium_client::disconnected() {
    qDebug()<<"Disconnected from server...\n";
    return;

}

void virgilium_client::bytesWritten(_int bytes) {
    qDebug()<< bytes << " written";
    return;
}
