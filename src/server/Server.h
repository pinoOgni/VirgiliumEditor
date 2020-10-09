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

    void incomingConnection(qintptr handle) override;

    void dispatch();

private slots:

    void onSocketStateChanged(QTcpSocket::SocketState state);

    void onProcessBasicMessage(_int code, BasicMessage basicMessage);

    void onProcessCrdtMessage(_int code, const CrdtMessage &crdtMessage);

    void onProcessUserMessage(_int code, UserMessage userMessage);

    void onProcessStorageMessage(_int code, StorageMessage storageMessage);

    void onFileManagementMessageReceived(_int code, FileManagementMessage fileManagementMessage);

    void onChangePasswordMessageReceived(_int code, ChangePasswordMessage changePasswordMessage);

    void onUserManagementMessageReceived(_int code, UserManagementMessage userManagementMessage);

    void onInvitationReceived(_int code, InvitationMessage invitationMessage);

    void onLogoutReceived(_int code); //pino close connection  DB user "logout"

signals:

    void processBasicMessage(_int code, BasicMessage basicMessage);

    void processCrdtMessage(_int code, CrdtMessage crdtMessage);

    void processUserMessage(_int code, UserMessage userMessage);

    void processFilesMessage(_int code, FilesMessage filesMessage);
};


#endif //VIRGILIUM_SERVER_H
