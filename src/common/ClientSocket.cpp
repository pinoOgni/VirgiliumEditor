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
    if(!this->in.commitTransaction()) {
        qDebug()<<"qualcosa e' andato storto con il clientID"<<this->clientID;
        return;
    }
    switch(code) {
        case CLIENT_CONNECTED: {
            BasicMessage bm;
            this->in >> bm;
            emit basicMessageReceived(code, bm);
        }
            break;
        case LOGIN:
        case SIGNUP:
        case GET_INFO_USER:
        case GET_FILES_OWNER:
        case GET_FILES_COLLABORATOR:
        case GET_ALL_DATA:{
            UserMessage um;
            this->in >> um;
            emit userMessageReceived(code, um);
            qDebug() << "code " << code;
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
            while(!in.atEnd()) {
                row++;
                in >> temp;
                filesMessage.push_back(temp);
            }
            emit filesMessageReceived(code,filesMessage);
        }
        case GET_ALL_DATA_OK: {
            UserMessage um = UserMessage();
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

            emit allDataReceived(code,um,row1,filesOwner,row2,filesCollabs);
        }
        break;
        case RENAME_FILE:
        case DELETE_FILE:
        case NEW_FILE: {
            qDebug() << "code " << code;
            FileManagementMessage fileManagementMessage;
            this->in >> fileManagementMessage;
            emit fileManagementMessageReceived(code,fileManagementMessage);
        }
        break;
        case RENAME_FILE_OK:
        case RENAME_FILE_KO:
        case DELETE_FILE_OK:
        case DELETE_FILE_KO:
        case NEW_FILE_OK:
        case NEW_FILE_KO:{
            emit fileManagementMessageResponse(code);
        }
        break;
        case CHANGE_PASSWORD: {
            ChangePasswordMessage changePasswordMessage;
            this->in >> changePasswordMessage;
            emit changePasswordMessageReceived(code,changePasswordMessage);
        }
        break;
        case CHANGE_PASSWORD_OK:
        case CHANGE_PASSWORD_KO: {
            emit changePasswordMessageResponse(code);
        }
        break;
        case CREATE_INVITE:
        case ADD_COLLABORATOR:
        case REMOVE_COLLABORATOR:
        case UNSUBSCRIBE: {
            UserManagementMessage userManagementMessage;
            this-> in >> userManagementMessage;
            emit userManagementMessageReceived(code,userManagementMessage);
        }
        break;
        case INVITE_CREATED:
        case REQUEST_TO_COLLABORATE: {
            InvitationMessage invitationMessage;
            this->in >> invitationMessage;
            emit invitationReceived(code,invitationMessage);
        }
        break;
        case ADD_COLLABORATOR_OK:
        case ADD_COLLABORATOR_KO:
        case REMOVE_COLLABORATOR_OK:
        case REMOVE_COLLABORATOR_KO:
        case UNSUBSCRIBE_OK:
        case UNSUBSCRIBE_KO: {
            emit userManagementMessageResponse(code);
        }
        case LOGOUT: {
            emit logoutReceived(LOGOUT);
        }
        break;
        case REQUEST_TO_COLLABORATE_OK:
        case REQUEST_TO_COLLABORATE_KO: {
            emit requestToCollaborateReceived(code);
        }
        break;
    }

}

ClientSocket::ClientSocket(QObject *parent) : QTcpSocket(parent), clientID(-1),in(this)  {
    connect(this,&QTcpSocket::readyRead,this,&ClientSocket::onReadyRead);
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

void ClientSocket::send(_int code, InvitationMessage invitationMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << code;
    out << invitationMessage;

    this->write(arrBlock);
}