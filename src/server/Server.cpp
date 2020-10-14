//
// Created by alex, pinoOgni on 10/08/20.
//

#include <common/ClientSocket.h>
#include <common/constants.h>
#include <common/messages/CrdtMessage.h>
#include <QtGui/QTextDocument>
#include <QTextCodec>
#include <common/messages/ActiveUserMessage.h>
#include "Server.h"

//ADD quint16 port è un unsigned short
Server::Server(unsigned short port, Model &model) : model(model) {
    if (!listen(QHostAddress::LocalHost, port)) {
        qDebug() << "Error: server is not listening" << "\n";
        exit(-1);
    }

    qDebug() << "Server is listening on address:" << this->serverAddress().toString() << ":" << this->serverPort()
             << "\n";

    //TODO PROVA PINO
    if (TESTDB == true) {
        QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(
                VIRGILIUM_STORAGE)).removeRecursively();
        QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)).mkdir("VIRGILIUM_STORAGE");
    }
}

void Server::incomingConnection(_int handle) {
    auto newSocket = new ClientSocket(this);
    if (!newSocket->setSocketDescriptor(handle)) {
        newSocket->deleteLater();
        return;
    }

    newSocket->setClientID(handle);

    connect(newSocket, &ClientSocket::basicMessageReceived, this, &Server::onProcessBasicMessage);
    connect(newSocket, &ClientSocket::userMessageReceived, this, &Server::onProcessUserMessage);
    connect(newSocket, &ClientSocket::fileManagementMessageReceived, this, &Server::onFileManagementMessageReceived);
    connect(newSocket, &ClientSocket::changePasswordMessageReceived, this, &Server::onChangePasswordMessageReceived);
    connect(newSocket, &ClientSocket::userManagementMessageReceived, this, &Server::onUserManagementMessageReceived);
    connect(newSocket, &ClientSocket::logoutReceived, this, &Server::onLogoutReceived);
    connect(newSocket, &ClientSocket::invitationReceived, this, &Server::onInvitationReceived);
    connect(newSocket, &ClientSocket::storageMessageReceived, this, &Server::onProcessStorageMessage);
    connect(newSocket, &ClientSocket::crdtMessageReceived, this, &Server::onProcessCrdtMessage);
    connect(newSocket, &ClientSocket::stateChanged, this, &Server::onSocketStateChanged);

    newSocket->setClientID(handle);

    //client is connected
    BasicMessage basicMessage(handle);
    qDebug() << "Sending: " << handle << "\n";
    newSocket->send(CLIENT_CONNECTED, basicMessage);
}

void Server::onSocketStateChanged(QTcpSocket::SocketState state) {
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
        case QAbstractSocket::ClosingState: {
            qDebug() << "The socket is about to close." << "\n";
            auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
            this->model.removeLoggedUser(sender);
            this->model.removeActiveUser(sender->getClientID());
            break;
        }
        case QAbstractSocket::ListeningState:
            qDebug() << "The socket is listening." << "\n";
            break;
        default:
            qDebug() << "Unknown State." << "\n";
    }
}

void Server::onProcessBasicMessage(_int code, BasicMessage basicMessage) {
    //robe super basic
    switch (code) {
        case CLIENT_CONNECTED:
            break;
    }
}

void Server::onProcessCrdtMessage(_int code, const CrdtMessage &crdtMessage) {
    this->model.insertMessage(crdtMessage);

    while (!this->model.getMessages().empty()) {
        auto message = this->model.getMessages().dequeue();
        auto activeUsersForDocument = this->model.getActiveClientsForDocument();
        auto it = activeUsersForDocument.find(crdtMessage.getFileName());
        if (it == activeUsersForDocument.end())
            return;

        /* It is a kind of dispatch messages */
        try {
            QList<User> users = activeUsersForDocument[crdtMessage.getFileName()];
            for (auto &user : users) {
                if (message.getSender() != user.getSiteId()) {
                    message.setMode(true);
                    ClientSocket *socket = this->model.getLoggedUser(user);
                    if (message.getAction() == "CURSOR_CHANGED")
                        socket->send(CURSOR_CHANGED, message);
                    else
                        socket->send(SYMBOL_INSERT_OR_ERASE, message);
                }
            }

            /* Now, the new symbol must be saved on file system */
            if (code == SYMBOL_INSERT_OR_ERASE)
                this->model.save(crdtMessage);
        } catch (std::exception &e) {
            qDebug() << e.what(); //TODO mettere in un file di log
            throw;
        }
    }
}

void Server::onProcessStorageMessage(_int code, StorageMessage storageMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    if (code == LOAD_REQUEST) {
        try {
            QList<User> users = model.addActiveUser(storageMessage.getActiveUsers().at(0),
                                                    storageMessage.getFileName());

            QVector<Symbol> symbols;
            if (users.size() == 1) {
                symbols = this->model.getFileFromFileSystem(storageMessage.getFileName());
                this->model.insertSymbolsForDocument(storageMessage.getFileName(), symbols);
            } else {
                symbols = this->model.getSymbolsForDocument(storageMessage.getFileName());
            }

            StorageMessage storageMessage1(0, symbols, storageMessage.getFileName(), users);
            sender->send(LOAD_RESPONSE, storageMessage1);

            for (auto &user : users) {
                if (storageMessage.getActiveUsers().at(0).getSiteId() != user.getSiteId()) {
                    ClientSocket *socket = this->model.getLoggedUser(user);
                    ActiveUserMessage activeUserMessage(0, users);
                    if (socket == nullptr) return;
                    socket->send(UPDATE_ACTIVE_USERS, activeUserMessage);
                }
            }
        } catch (std::exception &e) {
            qDebug() << e.what(); //TODO mettere in un file di log
            throw;
        }
    }
}

void Server::onProcessUserMessage(_int code, UserMessage userMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    qDebug() << "userMessage onProcessUserMessage " << userMessage.getUser().printMessage();

    switch (code) {
        case LOGIN: {
            qDebug() << "onProcessUserMessage LOGIN" << userMessage.getUser().getSiteId();

            bool isUserOnline = this->model.isUserOnline(userMessage.getUser().getEmail());
            if (isUserOnline) {
                qDebug() << "ALREADY_LOGGED";
                sender->send(ALREADY_LOGGED);
            } else {
                if (Model::loginUser(userMessage.getUser())) {
                    this->model.insertLoggedUser(sender, userMessage.getUser());
                    this->model.insertUserOnline(userMessage.getUser().getEmail());
                    sender->send(LOGIN_OK);
                    qDebug() << "readClient L true";
                } else {
                    sender->send(LOGIN_KO);
                    qDebug() << "readClient L false";
                }
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
        case DELETE_ACTIVE: {
            QList<User> users = this->model.removeActiveUser(userMessage.getUser(), userMessage.getFileName());

            //PINO 12 ottobre
            qDebug() << "onProcessUserMessage " << userMessage.getUser().getEmail() << " ---- " << userMessage.getFileName();
            QRegExp tagExp("/");
            QStringList firstList = userMessage.getFileName().split(tagExp);
            QString email_owner = firstList.at(0);
            QString filename = firstList.at(1);
            qDebug() << "onProcessUserMessage " << email_owner << " ---- " << filename;
            if(model.updateLastAcces(userMessage.getUser().getEmail(),
                                     model.getIdFilename(email_owner, filename))) {
                if(QString::compare(email_owner,userMessage.getUser().getEmail())==0) {
                    qDebug() << "update last_access owner";
                    User user = User(email_owner);
                    UserMessage userMessage = UserMessage(sender->getClientID(),user);
                    onProcessUserMessage(GET_FILES_OWNER,userMessage);
                } else {
                    qDebug() << "update last_access collaborator";
                    User user = User(userMessage.getUser().getEmail());
                    UserMessage userMessage = UserMessage(sender->getClientID(),user);
                    onProcessUserMessage(GET_FILES_COLLABORATOR,userMessage);
                }
            } else {
                qDebug() << "update last_access ERROR";
            }
            //fine pino

            if (users.empty()) {
                this->model.removeSymbolsForDocument(userMessage.getFileName());
                return;
            }

            ActiveUserMessage activeUserMessage(0, users);
            for (auto &user : users) {
                ClientSocket *socket = this->model.getLoggedUser(user);
                if (socket == nullptr) return;
                socket->send(UPDATE_ACTIVE_USERS, activeUserMessage);
            }
        }
            break;
        default: {

        }
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
        case CREATE_INVITE: {
            qDebug() << "create_invite: " << userManagementMessage.getEmail_owner() << "\n";
            QString invitationCode = model.createUrlCollaborator(userManagementMessage);
            InvitationMessage invitationMessage = InvitationMessage(sender->getClientID(),
                                                                    userManagementMessage.getEmail_owner(),
                                                                    invitationCode);
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

void Server::onLogoutReceived(_int code, UserMessage userMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    switch (code) {
        case LOGOUT: {
            qDebug() << "close connection DB";
            model.closeConnectionDB();
            //this->model.removeUserFromEditor(sender);
            this->model.removeLoggedUser(sender);
            this->model.removeUserOnline(userMessage.getUser().getEmail());
        }
            break;
    }
}

void Server::onInvitationReceived(_int code, InvitationMessage invitationMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    switch (code) {
        case REQUEST_TO_COLLABORATE: {
            qDebug() << "onInvitationReceived";
            if (model.requestToCollaborate(invitationMessage))
                sender->send(REQUEST_TO_COLLABORATE_OK);
            else
                sender->send(REQUEST_TO_COLLABORATE_KO);
        }
            break;
    }
}

/*void Server::onClosePendingSocket(_int clientID, ClientSocket *cs){
    if(clientID==cs->getClientID()){
       qDebug() << cs->getClientID() << "ciao2!";
       cs->deleteLater();
    }
}*/
