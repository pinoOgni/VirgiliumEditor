//
// Created by alex, pinoOgni & simod on 10/08/20.
//

#include "ClientSocket.h"
#include "constants.h"

/* server side constructor */
ClientSocket::ClientSocket(QObject *parent) : QTcpSocket(parent), clientID(-1), in(this) {
    //connect(this, &QTcpSocket::stateChanged, this, &ClientSocket::onSocketStateChanged);
    connect(this, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this, &ClientSocket::onDisplayError);
    connect(this, &QTcpSocket::disconnected, this, &ClientSocket::onDisconnectedSocketServer);
    connect(this, &QTcpSocket::bytesWritten, this, &ClientSocket::onBytesWritten);
    connect(this, &QTcpSocket::readyRead, this, &ClientSocket::onReadyRead);

    this->in.setVersion(Q_DATA_STREAM_VERSION);
}

/* client side constructor */
ClientSocket::ClientSocket(const QString &hostName, quint16 port, QObject *parent) :
        QTcpSocket(parent),
        clientID(-1),
        in(this) {
    connect(this, &QTcpSocket::connected, this, &ClientSocket::onConnected);
    connect(this, &QTcpSocket::disconnected, this, &ClientSocket::onDisconnected);
    connect(this, &QTcpSocket::bytesWritten, this, &ClientSocket::onBytesWritten);
    connect(this, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this,
            &ClientSocket::onDisplayError);   //need to overload the QAbstractSocket error:
    connect(this, &QTcpSocket::readyRead, this, &ClientSocket::onReadyRead);

    this->connectToHost(hostName, port);

    if (!this->waitForConnected(ConnectionWaitingTime)) { //5s
        //this->error();
    };

    this->in.setVersion(Q_DATA_STREAM_VERSION);
}

ClientSocket::~ClientSocket() {
    /*this->disconnectFromHost();
    this->waitForDisconnected(3000);*/
    //spdlog::debug("distructor");
}

void ClientSocket::onReadyRead() {
    this->in.startTransaction();
    _int code;
    this->in >> code;
    //spdlog::debug("ClientSocket onReadyRead {} ", code);

    switch (code) {
        case UPDATE_ACTIVE_USERS: {
            ActiveUserMessage activeUserMessage;
            this->in >> activeUserMessage;

            if (!this->in.commitTransaction()) return;
            emit activeUserMessageReceived(code, activeUserMessage);
        }
            break;
        case LOAD_RESPONSE: {
            StorageMessage storageMessage;
            this->in >> storageMessage;

            if (!this->in.commitTransaction()) return;
            emit storageMessageReceivedLoad(code, storageMessage);
        }
            break;
        case LOAD_REQUEST: {
            StorageMessage storageMessage;
            this->in >> storageMessage;

            if (!this->in.commitTransaction()) return;
            emit storageMessageReceived(code, storageMessage);
        }
            break;
        case CLIENT_CONNECTED: {
            BasicMessage bm;
            this->in >> bm;

            if (!this->in.commitTransaction()) return;
            emit basicMessageReceived(code, bm);
        }
            break;
        case LOGIN:
        case SIGNUP:
        case GET_INFO_USER:
        case GET_FILES_OWNER:
        case GET_FILES_COLLABORATOR:
        case DELETE_ACTIVE:
        case GET_ALL_DATA: {
            UserMessage um;
            this->in >> um;

            if (!this->in.commitTransaction()) return;
            emit userMessageReceived(code, um);
        }
            break;
        case LOGIN_KO:
        case LOGIN_OK:
        case SIGNUP_OK:
        case SIGNUP_KO:
        case ALREADY_LOGGED:{
            if (!this->in.commitTransaction()) return;
            emit loginSignupReceived(code);
        }
        break;
        case GET_FILES_OWNER_OK:
        case GET_FILES_OWNER_KO:
        case GET_FILES_COLLABORATOR_OK:
        case GET_FILES_COLLABORATOR_KO: {
            std::vector<FilesMessage> filesMessage;
            int row = 0;
            FilesMessage temp = FilesMessage();
            while (!in.atEnd()) {
                row++;
                in >> temp;
                filesMessage.push_back(temp);
            }
            if (!this->in.commitTransaction()) return;
            emit filesMessageReceived(code, filesMessage);
        }
            break;
        case GET_ALL_DATA_OK: {
            UserMessage um = UserMessage();
            in >> um;

            std::vector<FilesMessage> filesOwner;
            _int row1;
            in >> row1;
            FilesMessage temp1 = FilesMessage();
            for (int i = 0; i < row1; i++) {
                in >> temp1;
                filesOwner.push_back(temp1);
            }
            //spdlog::debug("GET_ALL_DATA row1 ");
            for (auto item: filesOwner)
                item.printUserInfo();

            std::vector<FilesMessage> filesCollabs;
            _int row2;
            in >> row2;
            FilesMessage temp2 = FilesMessage();
            for (int i = 0; i < row2; i++) {
                in >> temp2;
                filesCollabs.push_back(temp2);
            }

            //spdlog::debug("GET_ALL_DATA row2 ");
            for (auto item: filesCollabs)
                item.printUserInfo();
            if (!this->in.commitTransaction()) return;
            emit allDataReceived(code, um, row1, filesOwner, row2, filesCollabs);
        }
            break;
        case RENAME_FILE:
        case DELETE_FILE:
        case NEW_FILE: {
            FileManagementMessage fileManagementMessage;
            this->in >> fileManagementMessage;

            if (!this->in.commitTransaction()) return;
            emit fileManagementMessageReceived(code, fileManagementMessage);
        }
            break;
        case RENAME_FILE_OK:
        case RENAME_FILE_KO:
        case DELETE_FILE_OK:
        case DELETE_FILE_KO:
        case NEW_FILE_OK:
        case NEW_FILE_KO:
        case CANNOT_DELETE_FILE: {
            if (!this->in.commitTransaction()) return;
            emit fileManagementMessageResponse(code);
        }
            break;
        case CHANGE_PASSWORD: {
            ChangePasswordMessage changePasswordMessage;
            this->in >> changePasswordMessage;

            if (!this->in.commitTransaction()) return;
            emit changePasswordMessageReceived(code, changePasswordMessage);
        }
            break;
        case CHANGE_PASSWORD_OK:
        case CHANGE_PASSWORD_KO: {
            if (!this->in.commitTransaction()) return;
            emit changePasswordMessageResponse(code);
        }
            break;
        case CREATE_INVITE:
        case ADD_COLLABORATOR:
        case REMOVE_COLLABORATOR:
        case UNSUBSCRIBE:
        case CAN_OPEN_FILE: {
            UserManagementMessage userManagementMessage;
            this->in >> userManagementMessage;

            if (!this->in.commitTransaction()) return;
            emit userManagementMessageReceived(code, userManagementMessage);
        }
            break;
        case INVITE_CREATED:
        case REQUEST_TO_COLLABORATE: {
            InvitationMessage invitationMessage;
            this->in >> invitationMessage;

            if (!this->in.commitTransaction()) return;
            emit invitationReceived(code, invitationMessage);
        }
            break;
        case ADD_COLLABORATOR_OK:
        case ADD_COLLABORATOR_KO:
        case REMOVE_COLLABORATOR_OK:
        case REMOVE_COLLABORATOR_KO:
        case UNSUBSCRIBE_OK:
        case UNSUBSCRIBE_KO:
        case CANNOT_REMOVE_COLL:
        case CAN_OPEN_FILE_KO:
        case CAN_OPEN_FILE_OK: {
            if (!this->in.commitTransaction()) return;
            emit userManagementMessageResponse(code);
        }
            break;
        case LOGOUT: {
            UserMessage um;
            this->in >> um;
            if (!this->in.commitTransaction()) return;
            emit logoutReceived(LOGOUT,um);
        }
            break;
        case REQUEST_TO_COLLABORATE_OK:
        case REQUEST_TO_COLLABORATE_KO: {
            if (!this->in.commitTransaction()) return;
            emit requestToCollaborateReceived(code);
        }
            break;
        case CURSOR_CHANGED:
        case SYMBOL_INSERT_OR_ERASE: {
            CrdtMessage crdtMessage;
            this->in >> crdtMessage;

            //spdlog::debug("SYMBOL {} ", crdtMessage.getSymbol().getLetter().toStdString());

            if (!this->in.commitTransaction()) return;
            emit crdtMessageReceived(code, crdtMessage);
        }
            break;
        default: {
            //spdlog::debug("Default case in ReadyRead");
        }
    }

    if (!in.atEnd())
            emit readyRead();
}

/*void ClientSocket::onSocketStateChanged(QTcpSocket::SocketState state) {
    switch (state) {
        case QAbstractSocket::ListeningState:
            //spdlog::debug("The socket is listening.");
            break;
        case QAbstractSocket::UnconnectedState: {
            //spdlog::debug("The socket is not connected.");
            //ADD SPIA ROSSA
            break;
        }
        case QAbstractSocket::HostLookupState:
            //spdlog::debug("The socket is performing a hostname lookup.");
            break;
        case QAbstractSocket::ConnectedState: {
            //spdlog::debug("A connection is established.");
            //ADD SPIA VERDE
            break;
        }
        case QAbstractSocket::ConnectingState:
            //spdlog::debug("The socket has started establishing a connection.");
            break;
        case QAbstractSocket::ClosingState: {
            //spdlog::debug("The socket is about to close.");
            auto sender = dynamic_cast<ClientSocket *>(QObject::sender());
            BasicMessage msg(sender->getClientID());
            break;
        }
        default:
            //spdlog::debug("Unknown State.");
    }
}*/

//solo per debug
void ClientSocket::onConnected() {
    //spdlog::debug("Connected.");
}

void ClientSocket::onDisconnected() {
    //spdlog::debug("disconnected.");
}

void ClientSocket::onDisconnectedSocketServer() {
    //spdlog::debug("disconnected.");
    this->deleteLater();
}

//solo per debug
void ClientSocket::onBytesWritten(_int bytes) {
    //spdlog::debug("bytes {} written ", bytes );
}

//add gli altri switch case
void ClientSocket::onDisplayError(QTcpSocket::SocketError error) {
    switch (error) {
        case QAbstractSocket::RemoteHostClosedError:  //l'host cade
            //spdlog::debug("RemoteHostClosedError: the remote host closed the connection.");
            break;
        case QAbstractSocket::HostNotFoundError: //address ""
            //spdlog::debug("HostNotFoundError: the host address was not found.");
            break;
        case QAbstractSocket::SocketAccessError:
            //spdlog::debug("SocketAccessError: the socket operation failed because the application lacked the required privileges.");
            break;
        case QAbstractSocket::SocketResourceError:
            //spdlog::debug("SocketResourceError: the local system ran out of resources (e.g., too many sockets).");
            break;
        case QAbstractSocket::ConnectionRefusedError: //se provo a connetermi ma il server è down
            //spdlog::debug("ConnectionRefusedError: the connection was refused by the peer (or timed out).");
            break;
        case QAbstractSocket::NetworkError:
            //spdlog::debug("NetworkError: an error occurred with the network.");
            break;
        case QAbstractSocket::OperationError:
            //spdlog::debug("OperationError: an operation was attempted while the socket was in a state that did not permit it.");
            break;
        case QAbstractSocket::UnknownSocketError:
            //spdlog::debug("UnknownSocketError: an unidentified error occurred..");
            break;
    }

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

void ClientSocket::setClientID(quintptr clientId) {
    this->clientID = clientId;
}

quint32 ClientSocket::getClientID() {
    return this->clientID;
}

void ClientSocket::send(_int code, BasicMessage basicMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(Q_DATA_STREAM_VERSION);

    out << code;
    out << basicMessage;
    this->write(arrBlock);
}

void ClientSocket::send(_int res) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(Q_DATA_STREAM_VERSION);

    out << res;
    this->write(arrBlock);
}

void ClientSocket::send(_int res, UserMessage userMessage) {
    QByteArray blocco;
    QDataStream out(&blocco, QIODevice::WriteOnly);
    out.setVersion(Q_DATA_STREAM_VERSION);

    out << res;
    out << userMessage;
    this->write(blocco);
}

void ClientSocket::send(_int code, std::vector<FilesMessage> filesMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(Q_DATA_STREAM_VERSION);

    out << code;
    for (auto item: filesMessage)
        out << item;

    this->write(arrBlock);
}

void ClientSocket::send(_int code, UserMessage userMessageReturn, std::vector<FilesMessage> filesOwner,
                        std::vector<FilesMessage> filesCollabs) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(Q_DATA_STREAM_VERSION);
    out << code;
    out << userMessageReturn;

    _int row1 = filesOwner.size();
    out << row1;
    for (auto item: filesOwner)
        out << item;

    _int row2 = filesCollabs.size();
    out << row2;
    for (auto item: filesCollabs)
        out << item;

    this->write(arrBlock);
}

void ClientSocket::send(_int code, FileManagementMessage fileManagementMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(Q_DATA_STREAM_VERSION);

    out << code;
    out << fileManagementMessage;
    this->write(arrBlock);
}

void ClientSocket::send(_int code, ChangePasswordMessage changePasswordMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(Q_DATA_STREAM_VERSION);

    out << code;
    out << changePasswordMessage;
    this->write(arrBlock);
}

void ClientSocket::send(_int code, UserManagementMessage userManagementMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(Q_DATA_STREAM_VERSION);

    out << code;
    out << userManagementMessage;
    this->write(arrBlock);
}

void ClientSocket::send(_int code, StorageMessage &storageMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(Q_DATA_STREAM_VERSION);

    out << code;
    out << storageMessage;
    this->write(arrBlock);
}

void ClientSocket::send(_int code, InvitationMessage invitationMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(Q_DATA_STREAM_VERSION);

    out << code;
    out << invitationMessage;
    this->write(arrBlock);
}

void ClientSocket::send(_int code, const CrdtMessage &crdtMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(Q_DATA_STREAM_VERSION);

    out << code;
    out << crdtMessage;
    this->write(arrBlock);
}

void ClientSocket::send(_int code, const ActiveUserMessage &activeUserMessage) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(Q_DATA_STREAM_VERSION);

    out << code;
    out << activeUserMessage;
    this->write(arrBlock);
}
