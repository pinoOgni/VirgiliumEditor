//
// Created by pinoOgni on 10/08/20.
//

#include "clientstuff.h"

ClientStuff::ClientStuff(
        const QString &hostName,
        quint16 port,
        QObject *parent
) : QObject(parent) {
    /*status = false;

    host = hostAddress;
    port = portNumber;*/

    clientSocket = new ClientSocket(hostName, port, parent);

    connect(clientSocket, &ClientSocket::basicMessageReceived, this, &ClientStuff::processBasicMessage);
    connect(clientSocket, &ClientSocket::loginSignupReceived, this, &ClientStuff::processLoginAndSignup);
    connect(clientSocket, &ClientSocket::userMessageReceived, this, &ClientStuff::processUserMessage);
    connect(clientSocket, &ClientSocket::filesMessageReceived, this, &ClientStuff::processFilesMessage);
    connect(clientSocket, &ClientSocket::allDataReceived, this, &ClientStuff::processAllData);
    connect(clientSocket, &ClientSocket::fileManagementMessageResponse, this, &ClientStuff::processFileManagement);
    connect(clientSocket, &ClientSocket::changePasswordMessageResponse, this, &ClientStuff::processChangePassword);
    connect(clientSocket, &ClientSocket::userManagementMessageResponse, this, &ClientStuff::processUserManagement);
    connect(clientSocket, &ClientSocket::invitationReceived, this, &ClientStuff::processInvitation);
    connect(clientSocket, &ClientSocket::requestToCollaborateReceived, this, &ClientStuff::processRequestToCollaborate);

    //clientSocket->connectToHost(host,port);
}

void ClientStuff::processAllData(_int code, UserMessage userMessage,
                                 _int row1, std::vector<FilesMessage> filesOwner,
                                 _int row2, std::vector<FilesMessage> filesCollabs) {
    //spdlog::debug("processAllData");
    switch (code) {
        case GET_ALL_DATA_OK: {
            FilesMessage temp1 = FilesMessage();
            for (auto item: filesOwner)
                item.printUserInfo();
            FilesMessage temp2 = FilesMessage();
            for (auto item: filesCollabs)
                item.printUserInfo();

            emit getAllData(userMessage, row1, filesOwner, row2, filesCollabs);
            //spdlog::debug("emit getAllData");
        }
            break;
    }

}

void ClientStuff::processRequestToCollaborate(_int code) {
    switch (code) {
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
            //spdlog::debug("invite created client stuff");
            emit isInviteCreated(invitationMessage);
        }
    }
}

void ClientStuff::processFilesMessage(_int code, std::vector<FilesMessage> filesMessage) {
    switch (code) {
        case GET_FILES_OWNER_OK:
        case GET_FILES_OWNER_KO: {
            FilesMessage temp = FilesMessage();
            //spdlog::debug("GET_FILES_OWNER ");
            for (auto item: filesMessage)
                item.printUserInfo();
            emit getFilesOwner(filesMessage.size(), filesMessage);
        }
            break;
        case GET_FILES_COLLABORATOR_OK:
        case GET_FILES_COLLABORATOR_KO: {
            FilesMessage temp = FilesMessage();
            //spdlog::debug("GET_FILES_COLLABORATOR ");
            for (auto item: filesMessage)
                item.printUserInfo();
            emit getUserFiles(filesMessage.size(), filesMessage);
        }
            break;
    }
}

void ClientStuff::processUserMessage(_int code, UserMessage um) {
    switch (code) {
        case GET_INFO_USER: {
            this->clientSocket->setClientID(um.getSender());
            emit getInfoUser(um);
            //spdlog::debug("get_info_user ");
        }
            break;
    }
}

void ClientStuff::processBasicMessage(_int code, BasicMessage bm) {
    switch (code) {
        case CLIENT_CONNECTED: {
            this->clientSocket->setClientID(bm.getSender());
            //spdlog::debug("client_connected  {}" ,this->clientSocket->getClientID());
        }
            break;
    }
}

void ClientStuff::processLoginAndSignup(_int code) {
    switch (code) {
        case LOGIN_OK: {
            emit tryToLogin(true);
            //spdlog::debug("login ok  {}", this->clientSocket->getClientID());
        }
            break;
        case LOGIN_KO: {
            emit tryToLogin(false);
            //spdlog::debug("login ko {}", this->clientSocket->getClientID());
        }
            break;
        case SIGNUP_OK: {
            emit tryToSignup(true);
            //spdlog::debug("signup ok  {}", this->clientSocket->getClientID());
        }
            break;
        case SIGNUP_KO: {
            emit tryToSignup(false);
            //spdlog::debug("signup ko {}", this->clientSocket->getClientID());
        }
            break;
        case ALREADY_LOGGED: {
            emit userAlreadyLogged();
            //spdlog::debug("userAlreadyLogged  {}", this->clientSocket->getClientID());
        }
            break;
    }
}

void ClientStuff::processFileManagement(_int code) {
    switch (code) {
        case RENAME_FILE_OK: {
            //spdlog::debug("rename file OK");
            emit isFileRenamed(true);
        }
            break;
        case RENAME_FILE_KO: {
            //spdlog::debug("rename file KO");
            emit isFileRenamed(false);
        }
            break;
        case DELETE_FILE_OK: {
            //spdlog::debug("delete file OK");
            emit isFileDeleted(true);
        }
            break;
        case DELETE_FILE_KO: {
            //spdlog::debug("delete file KO");
            emit isFileDeleted(false);
        }
            break;
        case CANNOT_DELETE_FILE: {
            qDebug("cannot delete file");
            emit canDeleteFile(false);
        }
            break;
        case NEW_FILE_OK: {
            //spdlog::debug("new file OK");
            emit isFileCreated(true);
        }
            break;
        case NEW_FILE_KO: {
            //spdlog::debug("new file KO");
            emit isFileCreated(false);
        }
            break;
    }
}


void ClientStuff::processChangePassword(_int code) {
    switch (code) {
        case CHANGE_PASSWORD_OK: {
            //spdlog::debug("change password OK");
            emit isPswChanged(true);
        }
            break;
        case CHANGE_PASSWORD_KO: {
            //spdlog::debug("change password KO");
            emit isPswChanged(false);
        }
            break;
    }
}

void ClientStuff::processUserManagement(_int code) {
    switch (code) {
        case ADD_COLLABORATOR_OK: {
            //spdlog::debug("add collaborator ok");
            emit isCollaboratorAdded(true);
        }
            break;
        case ADD_COLLABORATOR_KO: {
            //spdlog::debug("add collaborator ko");
            emit isCollaboratorAdded(false);
        }
            break;
        case REMOVE_COLLABORATOR_OK: {
            //spdlog::debug("remove collaborator ok");
            emit isCollaboratorRemoved(true);
        }
            break;
        case REMOVE_COLLABORATOR_KO: {
            //spdlog::debug("remove collaborator ko");
            emit isCollaboratorRemoved(false);
        }
            break;
        case UNSUBSCRIBE_OK: {
            //spdlog::debug(" unsubscribe ok");
            emit isUnsubscribed(true);
        }
            break;
        case UNSUBSCRIBE_KO: {
            //spdlog::debug(" unsubscribe ko");
            emit isUnsubscribed(false);
        }
            break;
        case CANNOT_REMOVE_COLL: {
            //spdlog::debug(" cannot remove collaborato");
            emit canRemoveCollaborator(false);
        }
            break;
        case CAN_OPEN_FILE_OK: {
            //spdlog::debug(" cannot open file");
            emit canOpenFile(true);
        }
            break;
        case CAN_OPEN_FILE_KO: {
            //spdlog::debug(" cannot open file");
            emit canOpenFile(false);
        }
            break;
    }
}

ClientSocket *ClientStuff::getSocket() {
    return this->clientSocket;
}

ClientStuff::~ClientStuff() {
    this->clientSocket->close();
    delete this->clientSocket; //svolge le operazioni dentro il distruttore della classe ClientSocket
    //spdlog::debug("client stuff ~");
}
