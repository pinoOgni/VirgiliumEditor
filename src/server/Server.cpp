//
// Created by alex on 10/08/20.
//

#include <common/ClientSocket.h>
#include <common/constants.h>
#include <common/messages/CrdtMessage.h>
#include <QtGui/QTextDocument>
#include <QTextCodec>
#include "Server.h"

Server::Server(unsigned short port, Model &model) : model(model) {
    if (!listen(QHostAddress::LocalHost, port)) {
        qDebug() << "Errore server";
        exit(-1);
    }

    qDebug() << "Listening on address:" << this->serverAddress().toString() << ":" << this->serverPort() << "\n";

    //TODO PROVA PINO
    if (!QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/storageVirgilium/")).exists())
        QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).mkdir("storageVirgilium");
}

void Server::incomingConnection(qintptr handle) {
    auto newSocket = new ClientSocket(this);
    if (!newSocket->setSocketDescriptor(handle)) {
        newSocket->deleteLater();
        return;
    }

    newSocket->setClientID(handle);

    connect(newSocket, &QTcpSocket::stateChanged, this, &Server::onSocketStateChanged);
    connect(newSocket, &ClientSocket::basicMessageReceived, this, &Server::onProcessBasicMessage);
    connect(newSocket, &ClientSocket::userMessageReceived, this, &Server::onProcessUserMessage);
    connect(newSocket, &ClientSocket::storageMessageReceived, this, &Server::onProcessStorageMessage);
    connect(newSocket, &ClientSocket::fileManagementMessageReceived, this, &Server::onFileManagementMessageReceived);
    connect(newSocket, &ClientSocket::changePasswordMessageReceived, this, &Server::onChangePasswordMessageReceived);
    connect(newSocket, &ClientSocket::userManagementMessageReceived, this, &Server::onUserManagementMessageReceived);

    //adesso dire al nuovo client che si è connesso
    BasicMessage basicMessage(handle);
    qDebug() << "Sending: " << handle << "\n";
    newSocket->send(CLIENT_CONNECTED, basicMessage);

    qDebug() << "ho mandato\n";
}

void Server::onSocketStateChanged(QTcpSocket::SocketState state) {

}


void Server::onProcessBasicMessage(_int code, BasicMessage basicMessage) {
    //robe super basic
    switch (code) {
        case CLIENT_CONNECTED:
            break;
    }

}

void Server::onProcessCrdtMessage(_int code, CrdtMessage crdtMessage) {
    //logica crdt
}

void Server::onProcessStorageMessage(_int code, StorageMessage storageMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    switch(code) {
        case LOAD_REQUEST: {
            if (!QFile::exists(storageMessage.getFileName()))
                return;

            QFile file(storageMessage.getFileName());
            if (!file.open(QFile::ReadOnly))
                return;

            QVector<Symbol> symbols;
            QDataStream in(&file);
            in >> symbols;
            file.close();

            StorageMessage storageMessage1(0, symbols, storageMessage.getFileName());
            sender->sendStorage(LOAD_RESPONSE, storageMessage1);
            break;
        }
        case SAVE: {
            if(!QDir("storage").exists())
                QDir().mkdir("storage");

            QRegExp tagExp("/");
            QStringList dataList = storageMessage.getFileName().split(tagExp);

            if(!QDir("storage/" + dataList[1]).exists())
                QDir().mkdir("storage/" + dataList[1]);

            QDir dir("storage/" + dataList[1]);
            QString filename = dataList[2];
            QFile file(dir.absoluteFilePath(filename));
            if (!file.open(QFile::WriteOnly))
                return;

            QDataStream out(&file);
            out << storageMessage.getSymbols();
            file.close();
            break;
        }
        default: {

        }
    }
}

void Server::dispatch() {

}

void Server::onProcessUserMessage(_int code, UserMessage userMessage) {
    // qDebug() << userMessage.getUser().printMessage();
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    qDebug() << "userMessage " << userMessage.getUser().printMessage();
    switch (code) {
        case LOGIN: {
            if (Model::loginUser(userMessage.getUser())) {
                sender->send(LOGIN_OK);
                qDebug() << "readClient L true";
            } else {
                sender->send(LOGIN_KO);
                qDebug() << "readClient L false";
            }
        }
            break;
        case SIGNUP: {
            if (Model::signinUser(userMessage.getUser())) {
                sender->send(SIGNUP_OK);
                qDebug() << "readClient S true";
            } else {
                sender->send(SIGNUP_KO);
                qDebug() << "readClient S false";
            }
        }
            break;
        case GET_INFO_USER: {
            User userReturn = model.getInfoUser(userMessage.getUser());
            // if(userReturn.getEmail().isEmpty())
            UserMessage userMessageReturn = UserMessage(sender->getClientID(), userReturn);
            sender->send(GET_INFO_USER, userMessageReturn);
        }
            break;
        case GET_FILES_OWNER: {
            std::vector<FilesMessage> filesMessage = model.getFilesOwner(userMessage.getUser());
            if (filesMessage.size() == 0) {
                qDebug() << "get_files_owner ko";
                sender->send(GET_FILES_OWNER_KO, filesMessage);
            } else {
                qDebug() << "get_files_owner ok";

                //problema con sender dove lo metto?
                for (int i = 0; i < filesMessage.size(); i++)
                    filesMessage[i].setSender(sender->getClientID());
                sender->send(GET_FILES_OWNER_OK, filesMessage);
            }
        }
            break;
        case GET_FILES_COLLABORATOR: {
            std::vector<FilesMessage> filesMessage = model.getUserFiles(userMessage.getUser());
            if (filesMessage.size() == 0) {
                qDebug() << "get_user_files ko";
                sender->send(GET_FILES_COLLABORATOR_OK, filesMessage);
            } else {
                qDebug() << "get_user_files ok";

                //problema con sender dove lo metto?
                for (int i = 0; i < filesMessage.size(); i++)
                    filesMessage[i].setSender(sender->getClientID());
                sender->send(GET_FILES_COLLABORATOR_KO, filesMessage);
            }
        }
            break;
        case GET_ALL_DATA: {
            User userReturn = model.getInfoUser(userMessage.getUser());
            // if(userReturn.getEmail().isEmpty())
            UserMessage userMessageReturn = UserMessage(sender->getClientID(), userReturn);

            std::vector<FilesMessage> filesOwner = model.getFilesOwner(userMessage.getUser());
            for (int i = 0; i < filesOwner.size(); i++)
                filesOwner[i].setSender(sender->getClientID());

            std::vector<FilesMessage> filesCollabs = model.getUserFiles(userMessage.getUser());
            for (int i = 0; i < filesCollabs.size(); i++)
                filesCollabs[i].setSender(sender->getClientID());

            sender->send(GET_ALL_DATA_OK, userMessageReturn, filesOwner, filesCollabs);
            qDebug() << "GET_ALL_DATA_OK ";
        }
            break;
    }
}


void Server::onFileManagementMessageReceived(_int code, FileManagementMessage fileManagementMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    qDebug() << "onFileManagementMessageReceived ";
    switch (code) {
        case RENAME_FILE: {
            qDebug() << "rename_file: " << fileManagementMessage.getEmail() << "\n";
            if (model.renameFile(fileManagementMessage))
                sender->send(RENAME_FILE_OK);
            else
                sender->send(RENAME_FILE_KO);
        }
            break;
        case DELETE_FILE: {
            qDebug() << "delete_file: " << fileManagementMessage.getEmail() << "\n";
            if (model.deleteFile(fileManagementMessage))
                sender->send(DELETE_FILE_OK);
            else
                sender->send(DELETE_FILE_KO);
        }
            break;
        case NEW_FILE: {
            qDebug() << "new_file: " << fileManagementMessage.getEmail() << "\n";
            if (model.newFile(fileManagementMessage))
                sender->send(NEW_FILE_OK);
            else
                sender->send(NEW_FILE_KO);
        }
            break;
    }
}


void Server::onChangePasswordMessageReceived(_int code, ChangePasswordMessage changePasswordMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    switch (code) {
        case CHANGE_PASSWORD: {
            qDebug() << "change_password: " << changePasswordMessage.getEmail() << "\n";
            if (model.changePassword(changePasswordMessage))
                sender->send(CHANGE_PASSWORD_OK);
            else
                sender->send(CHANGE_PASSWORD_KO);
        }
            break;
    }
}

void Server::onUserManagementMessageReceived(_int code, UserManagementMessage userManagementMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    switch (code) {
        case ADD_COLLABORATOR: {
            qDebug() << "add_collaborator: " << userManagementMessage.getEmail_owner() << "\n";
            if (model.addCollaborator(userManagementMessage))
                sender->send(ADD_COLLABORATOR_OK);
            else
                sender->send(ADD_COLLABORATOR_KO);
        }
            break;
        case REMOVE_COLLABORATOR: {
            qDebug() << "remove_collaborator: " << userManagementMessage.getEmail_owner() << "\n";
            if (model.removeCollaborator(userManagementMessage))
                sender->send(REMOVE_COLLABORATOR_OK);
            else
                sender->send(REMOVE_COLLABORATOR_KO);
        }
            break;
        case UNSUBSCRIBE: {
            qDebug() << "unsubscribe: " << userManagementMessage.getEmail_collaborator() << "\n";
            if (model.unsubscribe(userManagementMessage))
                sender->send(UNSUBSCRIBE_OK);
            else
                sender->send(UNSUBSCRIBE_KO);
        }
            break;
    }
}