//
// Created by alex on 07/12/19.
//

#include <common/ClientSocket.h>
#include <common/constants.h>
#include <common/messages/CrdtMessage.h>
#include <QtGui/QTextDocument>
#include <QTextCodec>
#include <common/messages/ActiveUserMessage.h>
#include "Server.h"

Server::Server(quint16 port, Model &model) : model(model) {
    if (!listen(QHostAddress::AnyIPv4, port)) {
        //spdlog::error("Error: server is not listening");
        std::cerr << "Error: server is not listening" << std::endl;
        exit(-1);
    }
    std::cout << "Listening on port " << port << std::endl;
    std::cout << "TESTDB = " << TESTDB << std::endl;

    freopen("serverLog.txt", "w", stderr);
    //spdlog::info("Server is listening on address: {0}, {1} ", this->serverAddress().toString().toStdString(), this->serverPort());
    if (TESTDB) {
        QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(
                VIRGILIUM_STORAGE)).removeRecursively();
        QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)).mkdir("VIRGILIUM_STORAGE");
    }
}

Server::~Server() {
    fclose(stderr);
};

/* This function is called when a new client is active and the client call the connectToHost method */
void Server::incomingConnection(_int handle) {
    auto newSocket = new ClientSocket(this);
    if (!newSocket->setSocketDescriptor(handle)) {
        newSocket->deleteLater();
        return;
    }

    newSocket->setClientID(handle);

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

    BasicMessage basicMessage(handle);
    //spdlog::debug("Sending: {} ", handle);
    newSocket->send(CLIENT_CONNECTED, basicMessage);
}

/* Every time the socket state change, this method is invoked */
void Server::onSocketStateChanged(QTcpSocket::SocketState state) {
    switch (state) {
        case QAbstractSocket::ClosingState: {
            //spdlog::debug("The socket is about to close.");
            auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
            this->model.removeActiveUser(sender->getClientID());
            this->model.removeUserOnline(sender->getClientID());
            this->model.removeLoggedUser(sender);
            break;
        }
        default:
            break;
    }
}

bool Server::checkUpdate(const CrdtMessage &crdtMessage) {
    if (crdtMessage.getAction() != "UPDATE")
        return false;

    auto font = crdtMessage.getSymbol().getFont().font;

    QRegExp tagExp(",");
    QStringList l = font.split(tagExp);
    if (l.at(0) != "-1" || l.at(1) != "-1")
        return true;

    for (const QString &s:l)
        if (s != "-1") return false;

    return true;
}

void Server::onProcessCrdtMessage(_int code, const CrdtMessage &crdtMessage) {
    this->model.insertMessage(crdtMessage);

    while (!this->model.getMessages().empty()) {
        auto message = this->model.getMessages().dequeue();
        auto activeUsersForDocument = this->model.getActiveClientsForDocument();

        QRegExp tagExp("/");
        QStringList firstList = crdtMessage.getFileName().split(tagExp);
        QString email_owner = firstList.at(0);
        QString filename = firstList.at(1);
        _int idFilename = model.getIdFilename(email_owner, filename);

        auto it = activeUsersForDocument.find(idFilename);
        if (it == activeUsersForDocument.end())
            return;

        /* It is a kind of dispatch messages */
        try {
            QList<User> users = activeUsersForDocument[idFilename];
            bool flag = checkUpdate(crdtMessage);
            for (auto &user : users) {
                if (message.getSender() != user.getSiteId() || flag) {
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
            //spdlog::error(e.what());
            std::cerr << e.what() << std::endl;
            throw;
        }
    }
}

void Server::onProcessStorageMessage(_int code, StorageMessage storageMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    if (code == LOAD_REQUEST) {
        try {
            QList<User> users = model.addActiveUserForDocument(storageMessage.getActiveUsers().at(0),
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
                    socket->send(UPDATE_ACTIVE_USERS, activeUserMessage);
                }
            }
        } catch (std::exception &e) {
            //spdlog::error(e.what());
            std::cerr << e.what() << std::endl;
            throw;
        }
    }
}

void Server::onProcessUserMessage(_int code, UserMessage userMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    //spdlog::debug("userMessage onProcessUserMessage {} ", userMessage.getUser().printMessage().toStdString());

    switch (code) {
        case LOGIN: {
            //spdlog::debug("onProcessUserMessage LOGIN {} ", userMessage.getUser().getSiteId());

            bool isUserOnline = this->model.isUserOnline(userMessage.getUser().getEmail());
            if (isUserOnline) {
                //spdlog::debug("ALREADY_LOGGED");
                sender->send(ALREADY_LOGGED);
            } else {
                if (Model::loginUser(userMessage.getUser())) {
                    this->model.insertLoggedUser(sender, userMessage.getUser());
                    this->model.insertUserOnline(sender->getClientID(), userMessage.getUser().getEmail());
                    sender->send(LOGIN_OK);
                    //spdlog::debug("readClient L true");
                } else {
                    sender->send(LOGIN_KO);
                    //spdlog::debug("readClient L false");
                }
            }
        }
            break;
        case SIGNUP: {
            if (Model::signinUser(userMessage.getUser())) {
                sender->send(SIGNUP_OK);
                //spdlog::debug("readClient S true");
            } else {
                sender->send(SIGNUP_KO);
                //spdlog::debug("readClient S false");
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
            if (filesMessage.empty()) {
                //spdlog::debug("get_files_owner ko");
                sender->send(GET_FILES_OWNER_KO, filesMessage);
            } else {
                //spdlog::debug("get_files_owner ok");
                for (auto &i : filesMessage)
                    i.setSender(sender->getClientID());
                sender->send(GET_FILES_OWNER_OK, filesMessage);
            }
        }
            break;
        case GET_FILES_COLLABORATOR: {
            std::vector<FilesMessage> filesMessage = model.getUserFiles(userMessage.getUser());
            if (filesMessage.empty()) {
                //spdlog::debug("get_user_files ko");
                sender->send(GET_FILES_COLLABORATOR_OK, filesMessage);
            } else {
                //spdlog::debug("get_user_files ok");
                for (auto &i : filesMessage)
                    i.setSender(sender->getClientID());
                sender->send(GET_FILES_COLLABORATOR_KO, filesMessage);
            }
        }
            break;
        case GET_ALL_DATA: {
            User userReturn = model.getInfoUser(userMessage.getUser());
            // if(userReturn.getEmail().isEmpty())
            UserMessage userMessageReturn = UserMessage(sender->getClientID(), userReturn);

            std::vector<FilesMessage> filesOwner = model.getFilesOwner(userMessage.getUser());
            for (auto &i : filesOwner)
                i.setSender(sender->getClientID());

            std::vector<FilesMessage> filesCollabs = model.getUserFiles(userMessage.getUser());
            for (auto &filesCollab : filesCollabs)
                filesCollab.setSender(sender->getClientID());

            sender->send(GET_ALL_DATA_OK, userMessageReturn, filesOwner, filesCollabs);
            //spdlog::debug("GET_ALL_DATA_OK ");
        }
            break;
        case DELETE_ACTIVE: {
            try {
                this->model.removeActiveUser(sender->getClientID());
                QList<User> users = this->model.removeActiveUserForDocument(userMessage.getUser(),
                                                                            userMessage.getFileName());

                QRegExp tagExp("/");
                QStringList firstList = userMessage.getFileName().split(tagExp);
                QString email_owner = firstList.at(0);
                QString filename = firstList.at(1);

                if (model.updateLastAccess(userMessage.getUser().getEmail(),
                                           model.getIdFilename(email_owner, filename))) {
                    if (QString::compare(email_owner, userMessage.getUser().getEmail()) == 0) {
                        User user = User(email_owner);
                        UserMessage userMessage1 = UserMessage(sender->getClientID(), user);
                        onProcessUserMessage(GET_FILES_OWNER, userMessage1);
                    } else {
                        User user = User(userMessage.getUser().getEmail());
                        UserMessage userMessage1 = UserMessage(sender->getClientID(), user);
                        onProcessUserMessage(GET_FILES_COLLABORATOR, userMessage1);
                    }
                } else {
                    //spdlog::error("update last_access ERROR");
                    std::cerr << "update last_access ERROR" << std::endl;
                }

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
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
                throw;
            }
        }
            break;
        default: {
            std::cerr << "Default case" << std::endl;
        }
    }
}

void Server::onFileManagementMessageReceived(_int code, const FileManagementMessage &fileManagementMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    //spdlog::debug("onFileManagementMessageReceived ");
    switch (code) {
        case RENAME_FILE: {
            //spdlog::debug("rename_file: {} ", fileManagementMessage.getEmail().toStdString());
            if (model.renameFile(fileManagementMessage))
                sender->send(RENAME_FILE_OK);
            else
                sender->send(RENAME_FILE_KO);
        }
            break;
        case DELETE_FILE: {
            //spdlog::debug("delete_file: {} ", fileManagementMessage.getEmail().toStdString());
            auto activeUsersForDocument = this->model.getActiveClientsForDocument();
            //getEmail in thi case return the email_owner
            _int idFilename = model.getIdFilename(fileManagementMessage.getEmail(),
                                                  fileManagementMessage.getFilename());

            auto it = activeUsersForDocument.find(idFilename);
            if (it != activeUsersForDocument.end()) {
                sender->send(CANNOT_DELETE_FILE);
            } else {
                if (model.deleteFile(fileManagementMessage))
                    sender->send(DELETE_FILE_OK);
                else
                    sender->send(DELETE_FILE_KO);
            }
        }
            break;
        case NEW_FILE: {
            //spdlog::debug("new_file: {}", fileManagementMessage.getEmail().toStdString());
            if (model.newFile(fileManagementMessage))
                sender->send(NEW_FILE_OK);
            else
                sender->send(NEW_FILE_KO);
        }
            break;
        default: {
            std::cerr << "Default case" << std::endl;
        }
    }
}


void Server::onChangePasswordMessageReceived(_int code, const ChangePasswordMessage &changePasswordMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    switch (code) {
        case CHANGE_PASSWORD: {
            //spdlog::debug("change_password: {} ", changePasswordMessage.getEmail().toStdString());
            if (model.changePassword(changePasswordMessage))
                sender->send(CHANGE_PASSWORD_OK);
            else
                sender->send(CHANGE_PASSWORD_KO);
        }
            break;
    }
}

void Server::onUserManagementMessageReceived(_int code, const UserManagementMessage &userManagementMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    switch (code) {
        case CREATE_INVITE: {
            //spdlog::debug("create_invite: {} ", userManagementMessage.getEmail_owner().toStdString());
            QString invitationCode = model.createUrlCollaborator(userManagementMessage);
            InvitationMessage invitationMessage = InvitationMessage(sender->getClientID(),
                                                                    userManagementMessage.getEmail_owner(),
                                                                    invitationCode);
            //spdlog::debug("CREATE INVITE SERVER ");
            sender->send(INVITE_CREATED, invitationMessage);
        }
            break;
        case REMOVE_COLLABORATOR: {
            //spdlog::debug("remove_collaborator: {} ", userManagementMessage.getEmail_owner().toStdString());

            auto activeUsersForDocument = this->model.getActiveClientsForDocument();
            _int idFilename = model.getIdFilename(userManagementMessage.getEmail_owner(),
                                                  userManagementMessage.getFilename());

            auto it = activeUsersForDocument.find(idFilename);
            bool tryToRemove = true;
            if (it != activeUsersForDocument.end()) {
                for (auto const &i : activeUsersForDocument.at(idFilename)) {
                    if (QString::compare(i.getEmail(), userManagementMessage.getEmail_collaborator()) == 0) {
                        tryToRemove = false;
                        sender->send(CANNOT_REMOVE_COLL);
                        break;
                    }
                }
            }

            if(tryToRemove) {
                if (model.removeCollaborator(userManagementMessage))
                    sender->send(REMOVE_COLLABORATOR_OK);
                else
                    sender->send(REMOVE_COLLABORATOR_KO);
            }
        }
            break;
        case UNSUBSCRIBE: {
            //spdlog::debug("unsubscribe: {} ", userManagementMessage.getEmail_collaborator().toStdString());
            if (model.unsubscribe(userManagementMessage))
                sender->send(UNSUBSCRIBE_OK);
            else
                sender->send(UNSUBSCRIBE_KO);
        }
            break;
        case CAN_OPEN_FILE: {
            //spdlog::debug("can open file");
            if (model.canOpenFile(userManagementMessage))
                sender->send(CAN_OPEN_FILE_OK);
            else
                sender->send(CAN_OPEN_FILE_KO);
        }
            break;
        default: {
            std::cerr << "Default case" << std::endl;
        }
    }
}

void Server::onLogoutReceived(_int code, UserMessage userMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    switch (code) {
        case LOGOUT: {
            //spdlog::debug("close connection DB");
            model.closeConnectionDB();
            try {
                this->model.removeActiveUser(sender->getClientID());
                this->model.removeUserOnline(sender->getClientID());
                this->model.removeLoggedUser(sender);
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
                throw;
            }
        }
            break;
    }
}

void Server::onInvitationReceived(_int code, InvitationMessage invitationMessage) {
    auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
    switch (code) {
        case REQUEST_TO_COLLABORATE: {
            //spdlog::debug("onInvitationReceived");
            if (model.requestToCollaborate(invitationMessage))
                sender->send(REQUEST_TO_COLLABORATE_OK);
            else
                sender->send(REQUEST_TO_COLLABORATE_KO);
        }
            break;
    }
}
