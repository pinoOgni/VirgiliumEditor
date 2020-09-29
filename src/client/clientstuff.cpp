//
// Created by pinoOgni on 10/08/20.
//

#include "clientstuff.h"

ClientStuff::ClientStuff(
        const QString hostAddress,
        int portNumber,
        QObject *parent
        ) : QObject(parent)
{
    status = false;

    host = hostAddress;
    port = portNumber;

    clientSocket = new ClientSocket();
    connect(clientSocket, &ClientSocket::basicMessageReceived, this, &ClientStuff::processBasicMessage);
    connect(clientSocket, &ClientSocket::loginSignupReceived,this, &ClientStuff::processLoginAndSignup);
    connect(clientSocket, &ClientSocket::userMessageReceived, this, &ClientStuff::processUserMessage);
    connect(clientSocket, &ClientSocket::filesMessageReceived, this, &ClientStuff::processFilesMessage);
    connect(clientSocket, &ClientSocket::allDataReceived,this,&ClientStuff::processAllData);
    connect(clientSocket, &ClientSocket::fileManagementMessageResponse,this,&ClientStuff::processFileManagement);
    connect(clientSocket, &ClientSocket::changePasswordMessageResponse,this,&ClientStuff::processChangePassword);
    connect(clientSocket, &ClientSocket::userManagementMessageResponse,this,&ClientStuff::processUserManagement);
    connect(clientSocket,&ClientSocket::invitationReceived, this, &ClientStuff::processInvitation);
    connect(clientSocket,&ClientSocket::requestToCollaborateReceived,this,&ClientStuff::processRequestToCollaborate);

    clientSocket->connectToHost(host,port);
}
void ClientStuff::processAllData(_int code, UserMessage userMessage,
                                 _int row1, std::vector<FilesMessage> filesOwner,
                                 _int row2, std::vector<FilesMessage> filesCollabs) {
qDebug() << "processAllData";
    switch (code) {
    case GET_ALL_DATA_OK: {
        FilesMessage temp1 = FilesMessage();
        qDebug() << "GET_ALL_DATA row1 " << row1;
        for(auto item: filesOwner)
            item.printUserInfo();
        FilesMessage temp2 = FilesMessage();
        qDebug() << "GET_ALL_DATA row2 " << row2;
        for(auto item: filesCollabs)
            item.printUserInfo();

        emit getAllData(userMessage,row1,filesOwner,row2,filesCollabs);
    }
    break;
    }

}

void ClientStuff::processRequestToCollaborate(_int code) {
    switch(code) {
        case REQUEST_TO_COLLABORATE_OK:
            emit isRequestToCollaboratedReceived(true);
            break;
        case REQUEST_TO_COLLABORATE_KO:
            emit isRequestToCollaboratedReceived(false);
            break;
    }
}

void ClientStuff::processInvitation(_int code, InvitationMessage invitationMessage) {
    switch (code) {
        case INVITE_CREATED: {
            qDebug() << "invite created client stuff";
            emit isInviteCreated(invitationMessage);
        }
    }
}

void ClientStuff::processFilesMessage(_int code, std::vector<FilesMessage> filesMessage) {
    switch(code) {
    case GET_FILES_OWNER_OK:
    case GET_FILES_OWNER_KO:{
        FilesMessage temp = FilesMessage();
        qDebug() << "GET_FILES_OWNER ";
        for(auto item: filesMessage)
            item.printUserInfo();
        emit getFilesOwner(filesMessage.size(),filesMessage);
    }
    break;
    case GET_FILES_COLLABORATOR_OK:
    case GET_FILES_COLLABORATOR_KO:{
        FilesMessage temp = FilesMessage();
        qDebug() << "GET_FILES_COLLABORATOR ";
        for(auto item: filesMessage)
            item.printUserInfo();
        emit getUserFiles(filesMessage.size(),filesMessage);
    }
    break;
    }
}

void ClientStuff::processUserMessage(_int code, UserMessage um) {
    switch (code) {
    case GET_INFO_USER: {
        this->clientSocket->setClientID(um.getSender());
        emit getInfoUser(um);
        qDebug()  << "get_info_user ";
    }
    break;
    }
}

void ClientStuff::processBasicMessage(_int code, BasicMessage bm) {
    switch (code) {
    case CLIENT_CONNECTED: {
        this->clientSocket->setClientID(bm.getSender());
        qDebug()  << "client_connected " << this->clientSocket->getClientID();
    }
    break;
    }
}

void ClientStuff::processLoginAndSignup(_int code) {
    switch (code) {
    case LOGIN_OK: {
        emit tryToLogin(true);
        qDebug()  << "login ok " << this->clientSocket->getClientID();
    }
    break;
    case LOGIN_KO: {
        emit tryToLogin(false);
        qDebug()  << "login ko" << this->clientSocket->getClientID();
    }
    break;
    case SIGNUP_OK: {
        emit tryToSignup(true);
        qDebug()  << "signup ok " << this->clientSocket->getClientID();
    }
    break;
    case SIGNUP_KO: {
        emit tryToSignup(false);
        qDebug()  << "signup ko" << this->clientSocket->getClientID();
    }
    break;
    }
}

void ClientStuff::processFileManagement(_int code) {
    switch (code) {
    case RENAME_FILE_OK: {
        qDebug() << "rename file OK";
        emit isFileRenamed(true);
    }
    break;
    case RENAME_FILE_KO: {
        qDebug() << "rename file KO";
        emit isFileRenamed(false);
    }
    break;
    case DELETE_FILE_OK: {
        qDebug() << "delete file OK";
        emit isFileDeleted(true);
    }
    break;
    case DELETE_FILE_KO: {
        qDebug() << "delete file KO";
        emit isFileDeleted(false);
    }
    break;
    case NEW_FILE_OK: {
        qDebug() << "new file OK";
        emit isFileCreated(true);
    }
    break;
    case NEW_FILE_KO: {
        qDebug() << "new file KO";
        emit isFileCreated(false);
    }
    break;
    }
}


void ClientStuff::processChangePassword(_int code) {
    switch (code) {
    case CHANGE_PASSWORD_OK: {
        qDebug() << "change password OK";
        emit isPswChanged(true);
    }
    break;
    case CHANGE_PASSWORD_KO: {
        qDebug() << "change password KO";
        emit isPswChanged(false);
    }
    break;
    }
}

void ClientStuff::processUserManagement(_int code) {
    switch (code) {
    case ADD_COLLABORATOR_OK: {
        qDebug() << "add collaborator ok";
        emit isCollaboratorAdded(true);
    }
    break;
    case ADD_COLLABORATOR_KO: {
        qDebug() << "add collaborator ko";
        emit isCollaboratorAdded(false);
    }
    break;
    case REMOVE_COLLABORATOR_OK: {
        qDebug() << "remove collaborator ok";
        emit isCollaboratorRemoved(true);
    }
    break;
    case REMOVE_COLLABORATOR_KO: {
        qDebug() << "remove collaborator ko";
        emit isCollaboratorRemoved(false);
    }
    break;
    case UNSUBSCRIBE_OK: {
        qDebug() << " unsubscribe ok";
        isUnsubscribed(true);
    }
    break;
    case UNSUBSCRIBE_KO: {
        qDebug() << " unsubscribe ko";
        isUnsubscribed(false);
    }
    break;
   }
}

ClientSocket * ClientStuff::getSocket() {
    return this->clientSocket;
}

ClientStuff::~ClientStuff() {
    this->clientSocket->close();
    delete this->clientSocket;
    qDebug() << "client stuff ~";
}
