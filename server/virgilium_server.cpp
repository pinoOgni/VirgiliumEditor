//
// Created by alex on 17/12/19.
//

#include "virgilium_server.h"

//#include "SharedEditor.h"
#include "virgilium_client.h"


virgilium_server::virgilium_server(QObject *parent): QObject(parent), IDs(1){
    this->server.setParent(this);

    connect(&this->server,&QTcpServer::newConnection,this,&virgilium_server::newConnection);
    //connect(&this->server,&QTcpServer::r)
    //questa classe contiene un oggetto QTcpServer e
    // con questa connect bindo il segnale newconnection
    // lanciato da qtcpserver
    // a il mio slot newConnection(), quello definito in questo file.
    if(!this->server.listen(QHostAddress::Any,LISTENING_PORT)){
        qDebug()<<"Server could not start";
    }
    else{
        qDebug()<<"Server started!\nListening on address: "<< this->server.serverAddress().toString()<<" and port: "<< this->server.serverPort()<<"\n";
    }


}

void virgilium_server::newConnection() {
    QByteArray blocco;
    //qua devo vedere che cosa devo fare
    QTcpSocket *nuovo = this->server.nextPendingConnection();
    virgilium_client *newClient = new virgilium_client(this,nuovo);

    connect(nuovo,&QTcpSocket::readyRead,newClient,&virgilium_client::readyRead);

    _int site_id = this->connect_to_client(newClient);
    newClient->setSiteId(site_id);

    this->editors.insert(std::pair<_int,virgilium_client*>(site_id,newClient));

    _int code=SITE_ID_ASSIGNMENT;
    QDataStream out(&blocco,QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_1);
    //connect(nuovo,&QAbstractSocket::disconnected,nuovo,&QObject::deleteLater); questa da togliere sennò uccide il socket
    out<<code;//la prima cosa che faccio sempre è mettere il codice a monte
    out<<site_id;//metto il site id che ho assegnato al nuovo client
    this->IDs++;//provare
    nuovo->write(blocco);
    return;
}


_int virgilium_server::connect_to_client(virgilium_client *client) {

    this->editors.insert(std::pair<_int,virgilium_client*>(this->IDs,client));
    //TODO
    /*Devo ragionare se questo incremento di this->IDs va fatto con un
     * lock
     * */
    return this->IDs;
}

void virgilium_server::send(const message &m) {
    this->messages.push(m);

}

void virgilium_server::dispatchmessages() {
    while(!this->messages.empty()){
        auto x = this->messages.front();
        for(auto iterator = this->editors.begin();iterator!=this->editors.end();iterator++){
            if(x.getFrom()!=iterator->second->getSiteId()){
                //TODO
                //fare robe
                iterator->second->sendMessage(x,SYMBOL_INSERT_OR_ERASE);

            }
        }
        this->messages.pop();
    }
    return;
}


