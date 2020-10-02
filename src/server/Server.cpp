//
// Created by alex, pinoOgni on 10/08/20.
//

#include <common/ClientSocket.h>
#include <common/constants.h>
#include <common/messages/CrdtMessage.h>
#include <QtGui/QTextDocument>
#include <QTextCodec>
#include "Server.h"

//ADD quint16 port è un unsigned short
Server::Server(unsigned short port, Model &model) : model(model) {


    if (!listen(QHostAddress::LocalHost, port)) {
        qDebug() << "Error: server is not listening" << "\n";
        exit(-1);
    }

    qDebug() << "Server is listening on address:" << this->serverAddress().toString() << ":" << this->serverPort() << "\n";

    //ale fai occhio
    //connect(this, &Server::processFilesMessage, this, &Server::onProcessFileMessage);

    //TODO PROVA PINO
    if(TESTDB==true) {
        QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE)).removeRecursively();
        QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)).mkdir("VIRGILIUM_STORAGE");
    }

    //if(!QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/VIRGILIUM_STORAGE")).exists())
}

void Server::incomingConnection(qintptr handle) {

    auto nuovoSocket = new ClientSocket(this);
    if (!nuovoSocket->setSocketDescriptor(handle)) {
        nuovoSocket->deleteLater();
        return;
    }
    nuovoSocket->setClientID(handle);

    /*connect(
            nuovoSocket,
            &QTcpSocket::stateChanged,
            this,
            &Server::onSocketStateChanged
    );*/

    connect(
            nuovoSocket,
            &ClientSocket::basicMessageReceived,
            this,
            &Server::onProcessBasicMessage
    );
    connect(
            nuovoSocket,
            &ClientSocket::userMessageReceived,
            this,
            &Server::onProcessUserMessage
    );
    connect(
            nuovoSocket,
            &ClientSocket::fileManagementMessageReceived,
            this,
            &Server::onFileManagementMessageReceived
    );
    connect(
            nuovoSocket,
            &ClientSocket::changePasswordMessageReceived,
            this,
            &Server::onChangePasswordMessageReceived
    );
    connect(
            nuovoSocket,
            &ClientSocket::userManagementMessageReceived,
            this,
            &Server::onUserManagementMessageReceived
    );
    connect(
            nuovoSocket,
            &ClientSocket::logoutReceived,
            this,
            &Server::onLogoutReceived
    );
    connect(
            nuovoSocket,
            &ClientSocket::invitationReceived,
            this,
            &Server::onInvitationReceived
    );

    connect(
            nuovoSocket,
            &ClientSocket::storageMessageReceived,
            this,
            &Server::onProcessStorageMessage);
    connect(
            nuovoSocket,
            &ClientSocket::crdtMessageReceived,
            this,
            &Server::onProcessCrdtMessage
            );

    nuovoSocket->setClientID(handle);


    //adesso dire al nuovo client che si è connesso
    BasicMessage basicMessage(handle);
    qDebug() << "Sending: " << handle << "\n";
    nuovoSocket->send(CLIENT_CONNECTED, basicMessage);

    qDebug() << "ho mandato\n";
}

/*void Server::onSocketStateChanged(QTcpSocket::SocketState state) {

    switch (state) {
        case QAbstractSocket::UnconnectedState:
            qDebug() << "The socket is not connected." << "\n";
            break;
        case QAbstractSocket::HostLookupState:
            qDebug() << "The socket is performing a hostname lookup." << "\n";
            break;
        case QAbstractSocket::ConnectedState:
            qDebug() << "A connection is established." << "\n";
            break;
        case QAbstractSocket::ConnectingState:
            qDebug() << "The socket has started establishing a connection." << "\n";
            break;
        case QAbstractSocket::BoundState:
            qDebug() << "The socket is bound to an address and port." << "\n";
            break;
        case QAbstractSocket::ClosingState:
        {qDebug() << "The socket is about to close." << "\n";
            auto sender = dynamic_cast<ClientSocket*>(QObject::sender());
            BasicMessage msg(sender->getClientID());
            break;}

        case QAbstractSocket::ListeningState:
            qDebug() << "The socket is listening." << "\n";
            break;
        default:
            qDebug() << "Unknown State." << "\n";
    }

}*/

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
    qDebug() << "onProcessStorageMessage";
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    switch(code) {
        case LOAD_REQUEST: {
            QFile file (QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE)).filePath(storageMessage.getFileName()));

            if(!file.exists())
                return;

            //QString filenamePath = QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE)).filePath(storageMessage.getFileName());
            qDebug() << "onprocessStoragemessage load request" << storageMessage.getFileName();
            //QFile file(filenamePath);

            if (!file.open(QFile::ReadOnly))
                return;

            QVector<Symbol> symbols;
            QDataStream in(&file);
            in >> symbols;
            file.close();

            StorageMessage storageMessage1(0, symbols, storageMessage.getFileName());
            sender->sendStorage(LOAD_RESPONSE, storageMessage1);
        }
        break;
        case SAVE: {
            QRegExp tagExp("/");
            QStringList dataList = storageMessage.getFileName().split(tagExp);

            QString filenamePath =  QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE).append(dataList[0])+"/").filePath(dataList[1]);
            qDebug() << "onprocessStoragemessage save" << filenamePath;
            QFile file(filenamePath);
            if (!file.open(QFile::WriteOnly))
                return;

            QDataStream out(&file);
            out << storageMessage.getSymbols();
            file.close();
        }
        break;
        default: {

        }
    }
}

void Server::dispatch() {}


void Server::onProcessUserMessage(_int code, UserMessage userMessage) {
   // qDebug() << userMessage.getUser().printMessage();
   auto sender = dynamic_cast<ClientSocket*>(QObject::sender());
    qDebug() << "userMessage onProcessUserMessage " << userMessage.getUser().printMessage();

  switch (code) {
        case LOGIN: {
            qDebug()<<"popipopi qui arrivo";
            if (Model::loginUser(userMessage.getUser())) {
                sender->send(LOGIN_OK);
                this->model.insertActiveUser(sender,userMessage.getUser());
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
    auto sender = dynamic_cast<ClientSocket*>(QObject::sender());
    switch(code) {
        case CREATE_INVITE: {
            qDebug() << "create_invite: " << userManagementMessage.getEmail_owner() << "\n";
            QString invitationCode = model.createUrlCollaborator(userManagementMessage);
            InvitationMessage invitationMessage = InvitationMessage(sender->getClientID(),userManagementMessage.getEmail_owner(),invitationCode);
            qDebug() << "CREATE INVITE SERVER ";
            sender->send(INVITE_CREATED, invitationMessage);
        }
        break;
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

void Server::onLogoutReceived(_int code) {
    auto sender = dynamic_cast<ClientSocket*>(QObject::sender());
    switch (code) {
       case LOGOUT: {
           qDebug() << "close connection DB";
            model.closeConnectionDB();
            this->model.removeUserFromEditor(sender);
            this->model.removeActiveUser(sender);
        }
        break;
    }
}

void Server::onInvitationReceived(_int code, InvitationMessage invitationMessage) {
    auto sender = dynamic_cast<ClientSocket*>(QObject::sender());
    switch(code) {
        case REQUEST_TO_COLLABORATE: {
            qDebug() << "onInvitationReceived";
            if(model.requestToCollaborate(invitationMessage))
                sender->send(REQUEST_TO_COLLABORATE_OK);
            else
                sender->send(REQUEST_TO_COLLABORATE_KO);
        }
        break;
    }
}

/*void Server::onClosePendingSocket(qint32 clientID, ClientSocket *cs){
    if(clientID==cs->getClientID()){
       qDebug() << cs->getClientID() << "ciao2!";
       cs->deleteLater();
    }
}*/
