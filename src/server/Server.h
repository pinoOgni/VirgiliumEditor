//
// Created by alex on 10/08/20.
//

#ifndef VIRGILIUM_SERVER_H
#define VIRGILIUM_SERVER_H

#include <QtNetwork/QTcpServer>
#include <common/messages/UserMessage.h>
#include <server/model/Model.h>
#include "../common/messages/FilesMessage.h"
#include "../common/messages/CrdtMessage.h"

class Server : public QTcpServer {
Q_OBJECT

    Model &model;

public:
    Server(unsigned short port, Model &model);

    ~Server();

    void incomingConnection(qintptr handle) override;

private slots:

    void onSocketStateChanged(QTcpSocket::SocketState state);

    void onProcessCrdtMessage(_int code, const CrdtMessage &crdtMessage);

    void onProcessUserMessage(_int code, UserMessage userMessage);

    void onProcessStorageMessage(_int code, StorageMessage storageMessage);

    void onFileManagementMessageReceived(_int code, const FileManagementMessage& fileManagementMessage);

    void onChangePasswordMessageReceived(_int code, const ChangePasswordMessage& changePasswordMessage);

    void onUserManagementMessageReceived(_int code, const UserManagementMessage& userManagementMessage);

    void onInvitationReceived(_int code, InvitationMessage invitationMessage);

    void onLogoutReceived(_int code, UserMessage userMessage); //pino close connection  DB user "logout"

signals:

    void processBasicMessage(_int code, BasicMessage basicMessage);

    void processCrdtMessage(_int code, CrdtMessage crdtMessage);

    void processUserMessage(_int code, UserMessage userMessage);

    void processFilesMessage(_int code, FilesMessage filesMessage);
};


#endif //VIRGILIUM_SERVER_H
