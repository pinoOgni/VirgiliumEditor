//
// Created by alex on 10/08/20.
//

#ifndef VIRGILIUM_CLIENTSOCKET_H
#define VIRGILIUM_CLIENTSOCKET_H

#include "../common/messages/UserMessage.h"
#include "../common/messages/BasicMessage.h"
#include <QtNetwork/QTcpSocket>
#include <QDataStream>
#include <common/messages/StorageMessage.h>
#include <common/messages/CrdtMessage.h>
#include <common/messages/ActiveUserMessage.h>
#include "../common/messages/InvitationMessage.h"
#include "../common/messages/FilesMessage.h"
#include "../common/messages/FileManagementMessage.h"
#include "../common/messages/ChangePasswordMessage.h"
#include "../common/messages/UserManagementMessage.h"
#include "constants.h"

class ClientSocket : public QTcpSocket {
Q_OBJECT
private:

    quint32 clientID;
    QDataStream in;

public:
    //socket lato server
    explicit ClientSocket(QObject *parent = nullptr);

    //socket lato client
    explicit ClientSocket(const QString &hostName, quint16 port, QObject *parent = nullptr);

    //~ClientSocket() override { qDebug() << "ClientSocket ~"; };
    ~ClientSocket();

    bool operator==(const ClientSocket &b);

    bool operator==(const ClientSocket *b);

    void send(QByteArray &data);

    void send(_int code, BasicMessage basicMessage);

    void send(_int code, UserMessage userMessage);

    void send(_int code, StorageMessage &storageMessage);

    void send(_int res);

    void send(_int code, std::vector<FilesMessage> filesMessage);

    void send(_int code, FileManagementMessage fileManagementMessage);

    void send(_int code, UserMessage userMessageReturn, std::vector<FilesMessage> filesOwner,
              std::vector<FilesMessage> filesCollabs);

    void send(_int code, ChangePasswordMessage changePasswordMessage);

    void send(_int code, InvitationMessage invitationMessage);

    void send(_int code, UserManagementMessage userManagementMessage);

    void send(_int code, const CrdtMessage &crdtMessage);

    void send(_int code, const ActiveUserMessage &activeUserMessage);

    void setClientID(quintptr clientID);

    quint32 getClientID();

private slots:

    void onReadyRead();

    //void onSocketStateChanged(QTcpSocket::SocketState state);

    void onConnected();

    void onDisconnected();

    void onDisconnectedSocketServer();

    void onBytesWritten(_int bytes);

    void onDisplayError(QTcpSocket::SocketError error);

signals:

    void basicMessageReceived(_int code, BasicMessage basicMessage);

    void userMessageReceived(_int code, UserMessage userMessage);

    void storageMessageReceived(_int code, StorageMessage storageMessage);

    void storageMessageReceivedLoad(_int code, StorageMessage storageMessage);

    void loginSignupReceived(_int code);

    void filesMessageReceived(_int code, std::vector<FilesMessage> filesMessage);

    void fileManagementMessageReceived(_int code, FileManagementMessage fileManagementMessage);

    void fileManagementMessageResponse(_int code);

    void allDataReceived(_int code, UserMessage userMessage, _int row1, std::vector<FilesMessage> filesOwner, _int row2,
                         std::vector<FilesMessage> filesCollabs);

    void changePasswordMessageReceived(_int code, ChangePasswordMessage changePasswordMessage);

    void changePasswordMessageResponse(_int code);

    void userManagementMessageReceived(_int code, UserManagementMessage userManagementMessage);

    void userManagementMessageResponse(_int code);

    void invitationReceived(_int code, InvitationMessage invitationMessage);

    void requestToCollaborateReceived(_int code);

    void logoutReceived(_int code,UserMessage userMessage); //pino close connection when client "logout"
    void crdtMessageReceived(_int code, CrdtMessage crdtMessage);

    void activeUserMessageReceived(_int code, ActiveUserMessage activeUserMessage);
};


#endif //VIRGILIUM_CLIENTSOCKET_H
