//
// Created by alex & pinoOgni on 10/08/20.
//


#include "ClientSocket.h"
#include "constants.h"
void ClientSocket::onReadyRead() {

    this->in.startTransaction();
    _int code;
    this->in >> code;
    qDebug() << "ClientSocket onreadyRead " << code;
    /*if(!this->in.commitTransaction()) {
        qDebug()<<"qualcosa e' andato storto con il clientID"<<this->clientID;
        return;
    }*/
    switch(code) {
        case LOAD_RESPONSE: {
            StorageMessage storageMessage;
            //this->in.startTransaction();
            this->in >> storageMessage;
            if(this->in.commitTransaction())
            emit storageMessageReceivedLoad(storageMessage);
        }
        break;
        case LOAD_REQUEST:
        case SAVE: {
            StorageMessage storageMessage;
            //this->in.startTransaction();
            this->in >> storageMessage;
            if(this->in.commitTransaction())
            emit storageMessageReceived(code, storageMessage);
            qDebug() << "cliensocket, save, load request, code " << code << " path" << storageMessage.getFileName();

        }
        break;
        case CLIENT_CONNECTED: {
            BasicMessage bm;
           // this->in.startTransaction();
            this->in >> bm;
            if(this->in.commitTransaction())
            emit basicMessageReceived(code, bm);
            qDebug()<<"Qui arrivo popopo";
        }
        break;
        case LOGIN:
        case SIGNUP:
        case GET_INFO_USER:
        case GET_FILES_OWNER:
        case GET_FILES_COLLABORATOR:
        case GET_ALL_DATA:{
            UserMessage um;
           // this->in.startTransaction();
            this->in >> um;
            auto res = this->in.commitTransaction();
            qDebug()<<"popoipoi" << um.getUser().printMessage();
            if(!res) return;
            emit userMessageReceived(code, um);
            qDebug() << "code " << code<<" res " << res;
        }
        break;
        case LOGIN_KO:
        case LOGIN_OK:
        case SIGNUP_OK:
        case SIGNUP_KO:{
            qDebug() << "ciao ale " << code;
            emit loginSignupReceived(code);
        }
        break;
        case GET_FILES_OWNER_OK:
        case GET_FILES_OWNER_KO:
        case GET_FILES_COLLABORATOR_OK:
        case GET_FILES_COLLABORATOR_KO:
        {
            std::vector<FilesMessage> filesMessage;
            int row = 0;
            FilesMessage temp = FilesMessage();
            //this->in.startTransaction();
            while(!in.atEnd()) {
                row++;
                in >> temp;
                filesMessage.push_back(temp);
            }
            if(this->in.commitTransaction())
            emit filesMessageReceived(code,filesMessage);
        }
        break;
        case GET_ALL_DATA_OK: {
            UserMessage um = UserMessage();
            //this->in.startTransaction();
            in >> um;

            std::vector<FilesMessage> filesOwner;
            _int row1;
            in >> row1;
            FilesMessage temp1 = FilesMessage();
            for(int i=0;i<row1;i++) {
                in >> temp1;
                filesOwner.push_back(temp1);
            }
            qDebug() << "GET_ALL_DATA row1 " << row1;
            for(auto item: filesOwner)
                item.printUserInfo();

            std::vector<FilesMessage> filesCollabs;
            _int row2;
            in >> row2;
            FilesMessage temp2 = FilesMessage();
            for(int i=0;i<row2;i++) {
                in >> temp2;
                filesCollabs.push_back(temp2);
            }

            qDebug() << "GET_ALL_DATA row2 " << row2;
            for(auto item: filesCollabs)
                item.printUserInfo();
            if(this->in.commitTransaction())
            emit allDataReceived(code,um,row1,filesOwner,row2,filesCollabs);
        }
        break;
        case RENAME_FILE:
        case DELETE_FILE:
        case NEW_FILE: {
            qDebug() << "code " << code;
            FileManagementMessage fileManagementMessage;
            //this->in.startTransaction();
            this->in >> fileManagementMessage;
            if(this->in.commitTransaction())
            emit fileManagementMessageReceived(code,fileManagementMessage);
        }
        break;
        case RENAME_FILE_OK:
        case RENAME_FILE_KO:
        case DELETE_FILE_OK:
        case DELETE_FILE_KO:
        case NEW_FILE_OK:
        case NEW_FILE_KO:{
            this->in.commitTransaction();
            emit fileManagementMessageResponse(code);
        }
        break;
        case CHANGE_PASSWORD: {
            ChangePasswordMessage changePasswordMessage;
           // this->in.startTransaction();
            this->in >> changePasswordMessage;
            if(this->in.commitTransaction())
            emit changePasswordMessageReceived(code,changePasswordMessage);
        }
        break;
        case CHANGE_PASSWORD_OK:
        case CHANGE_PASSWORD_KO: {
            this->in.commitTransaction();
            emit changePasswordMessageResponse(code);
        }
        break;
        case CREATE_INVITE:
        case ADD_COLLABORATOR:
        case REMOVE_COLLABORATOR:
        case UNSUBSCRIBE: {
            UserManagementMessage userManagementMessage;
            //this->in.startTransaction();
            this-> in >> userManagementMessage;
            if(this->in.commitTransaction())
            emit userManagementMessageReceived(code,userManagementMessage);
        }
        break;
        case INVITE_CREATED:
        case REQUEST_TO_COLLABORATE: {
            InvitationMessage invitationMessage;
            //this->in.startTransaction();
            this->in >> invitationMessage;
            if(this->in.commitTransaction())
            emit invitationReceived(code,invitationMessage);
        }
        break;
        case ADD_COLLABORATOR_OK:
        case ADD_COLLABORATOR_KO:
        case REMOVE_COLLABORATOR_OK:
        case REMOVE_COLLABORATOR_KO:
        case UNSUBSCRIBE_OK:
        case UNSUBSCRIBE_KO: {
            this->in.commitTransaction();
            emit userManagementMessageResponse(code);
        }
        break;
        case LOGOUT: {
            this->in.commitTransaction();
            emit logoutReceived(LOGOUT);
        }
        break;
        case REQUEST_TO_COLLABORATE_OK:
        case REQUEST_TO_COLLABORATE_KO: {
            this->in.commitTransaction();
            emit requestToCollaborateReceived(code);
        }
        break;

        case SYMBOL_INSERT_OR_ERASE: {
            CrdtMessage crdtMessage;
           // this->in.startTransaction();
            this->in >> crdtMessage;
           if( this->in.commitTransaction())
            emit crdtMessageReceived(code,crdtMessage);
        }
    }

}



void ClientSocket::onSocketStateChanged(QTcpSocket::SocketState state) {


    switch (state) {
        case QAbstractSocket::ListeningState:
            qDebug() << "The socket is listening.";
            break;
        case QAbstractSocket::UnconnectedState:
        {
            qDebug() << "The socket is not connected.";
            //ADD SPIA ROSSA
            break;
        }
        case QAbstractSocket::HostLookupState:
            qDebug() << "The socket is performing a hostname lookup.";
            break;
        case QAbstractSocket::ConnectedState: {
            qDebug() << "A connection is established.";
            //ADD SPIA VERDE
            break;
        }
        case QAbstractSocket::ConnectingState:
            qDebug() << "The socket has started establishing a connection.";
            break;
        case QAbstractSocket::ClosingState:{
        qDebug() << "The socket is about to close.";
            auto sender = dynamic_cast<ClientSocket*>(QObject::sender());
            BasicMessage msg(sender->getClientID());
            break;
        }
        default:
            qDebug() << "Unknown State.";
    }
    return;
}

//solo per debug
void ClientSocket::onConnected() {
    qDebug() << "Connected.";
    return;
}


void ClientSocket::onDisconnected() {
    qDebug() << "disconnected.";
    return;
}

void ClientSocket::onDisconnectedSocketServer() {
    qDebug() << "disconnected.";
    this->deleteLater();
    return;
}


//solo per debug
void ClientSocket::onBytesWritten(_int bytes) {
    qDebug() << bytes << "bytes written";
    return;
}

//add gli altri switch case
void ClientSocket::onDisplayError(QTcpSocket::SocketError error){

    switch (error) {
        case QAbstractSocket::RemoteHostClosedError:  //l'host cade
            qDebug() << "RemoteHostClosedError: the remote host closed the connection.";
            break;
        case QAbstractSocket::HostNotFoundError: //address ""
            qDebug() << "HostNotFoundError: the host address was not found.";
            break;
        case QAbstractSocket::SocketAccessError:
            qDebug() << "SocketAccessError: the socket operation failed because the application lacked the required privileges.";
            break;
        case QAbstractSocket::SocketResourceError:
            qDebug() << "SocketResourceError: the local system ran out of resources (e.g., too many sockets).";
            break;
        case QAbstractSocket::ConnectionRefusedError: //se provo a connetermi ma il server è down
            qDebug() << "ConnectionRefusedError: the connection was refused by the peer (or timed out).";
            break;
        case QAbstractSocket::NetworkError:
            qDebug() << "NetworkError: an error occurred with the network.";
            break;
        case QAbstractSocket::OperationError:
            qDebug() << "OperationError: an operation was attempted while the socket was in a state that did not permit it.";
            break;
        case QAbstractSocket::UnknownSocketError:
            qDebug() << "UnknownSocketError: an unidentified error occurred..";
            break;
    }

}

//socket lato server
//connected non serve perché non si fa la connecttohost
ClientSocket::ClientSocket(QObject *parent) : QTcpSocket(parent), clientID(-1),in(this)  {

    connect(this, &QTcpSocket::stateChanged,this,&ClientSocket::onSocketStateChanged);
    connect(this, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),this,&ClientSocket::onDisplayError);
    connect(this, &QTcpSocket::disconnected, this, &ClientSocket::onDisconnectedSocketServer);
    connect(this, &QTcpSocket::bytesWritten, this, &ClientSocket::onBytesWritten);
    connect(this,&QTcpSocket::readyRead,this,&ClientSocket::onReadyRead);

    this->in.setVersion(Q_DATA_STREAM_VERSION);

}

//socket lato client
ClientSocket::ClientSocket(const QString &hostName, quint16 port,QObject *parent) : QTcpSocket(parent), clientID(-1),in(this)  {

    connect(this, &QTcpSocket::stateChanged,this,&ClientSocket::onSocketStateChanged);
    connect(this, &QTcpSocket::connected, this, &ClientSocket::onConnected);
    connect(this, &QTcpSocket::disconnected, this, &ClientSocket::onDisconnected);
    connect(this, &QTcpSocket::bytesWritten, this, &ClientSocket::onBytesWritten);
    connect(this, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error),this,&ClientSocket::onDisplayError);   //need to overload the QAbstractSocket error:
    connect(this,&QTcpSocket::readyRead,this,&ClientSocket::onReadyRead);

    this->connectToHost(hostName,port);


    if(!this->waitForConnected(ConnectionWaitingTime)){ //5s
        //this->error();
    };


    this->in.setVersion(Q_DATA_STREAM_VERSION);

}

bool ClientSocket::operator==(const ClientSocket &b) {
    return this->clientID == b.clientID;
}

bool ClientSocket::operator==(const ClientSocket *b) {
    return this->clientID == b->clientID;
}

void ClientSocket::send(QByteArray &data) {
    this->write(data);
}

void ClientSocket::setClientID(quintptr clientID) {
    this->clientID=clientID;
}

quint32 ClientSocket::getClientID() {
    return this->clientID;
}

void ClientSocket::send(_int code, BasicMessage basicMessage) {
    QByteArray blocco;
    QDataStream out(&blocco, QIODevice::WriteOnly);

    out << code;
    out << basicMessage;

    this->write(blocco);
}

void ClientSocket::send(_int res) {
    QByteArray blocco;
    QDataStream out(&blocco, QIODevice::WriteOnly);

    out << res;

    this->write(blocco);
}

void ClientSocket::send(_int res, UserMessage userMessage) {
    QByteArray blocco;
    QDataStream out(&blocco, QIODevice::WriteOnly);

    out << res;
    out << userMessage;

    this->write(blocco);
}

void ClientSocket::send(_int code, std::vector<FilesMessage> filesMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << code;
    for(auto item: filesMessage)
        out << item;

    this->write(arrBlock);
}

void ClientSocket::send(_int code, UserMessage userMessageReturn, std::vector<FilesMessage> filesOwner, std::vector<FilesMessage> filesCollabs) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << code;
    out << userMessageReturn;

    _int row1 = filesOwner.size();
    out << row1;
    for(auto item: filesOwner)
        out << item;

    _int row2 = filesCollabs.size();
    out << row2;
    for(auto item: filesCollabs)
        out << item;

    this->write(arrBlock);
}

void ClientSocket::send(_int code, FileManagementMessage fileManagementMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << code;
    out << fileManagementMessage;

    this->write(arrBlock);
}

void ClientSocket::send(_int code, ChangePasswordMessage changePasswordMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << code;
    out << changePasswordMessage;

    this->write(arrBlock);
}

void ClientSocket::send(_int code, UserManagementMessage userManagementMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << code;
    out << userManagementMessage;

    this->write(arrBlock);
}

void ClientSocket::sendStorage(_int code, StorageMessage &storageMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << code;
    out << storageMessage;
    this->write(arrBlock);
}
  

void ClientSocket::send(_int code, InvitationMessage invitationMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << code;
    out << invitationMessage;
    this->write(arrBlock);
}

void ClientSocket::send(_int code, CrdtMessage crdtMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << code;
    out << crdtMessage;
    this->write(arrBlock);
}

/*
 * void ClientSocket::sendCrdt(_int code, CrdtMessage &crdtMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << code;
    out << crdtMessage;
    this->write(arrBlock);
}
 */
