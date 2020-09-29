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
class Server: public QTcpServer {
    Q_OBJECT

    //qui i dati del modello
    Model& model;
    void incomingConnection(qintptr handle) override;
    void processMessage(ClientSocket *sender,QByteArray data);
private slots:
    void onMessageReceived(QByteArray data);

    void onSocketStateChanged(QTcpSocket::SocketState state);
    void onProcessBasicMessage(_int code,BasicMessage basicMessage);
    void onProcessCrdtMessage(_int code,CrdtMessage crdtMessage);
    void onProcessUserMessage(_int code,UserMessage userMessage);
    void onFileManagementMessageReceived(_int code, FileManagementMessage fileManagementMessage);
    void onChangePasswordMessageReceived(_int code, ChangePasswordMessage changePasswordMessage);
    void onUserManagementMessageReceived(_int code, UserManagementMessage userManagementMessage);
    //void onProcessFileMessage(_int code, FilesMessage fileMessage);
    void onInvitationReceived(_int code, InvitationMessage invitationMessage);
    void onLogoutReceived(_int code); //pino close connection  DB user "logout"




public:
    Server(unsigned short port, Model& model);
    void dispatch();
signals:
    void processBasicMessage(_int code,BasicMessage basicMessage);
    void processCrdtMessage(_int code, CrdtMessage crdtMessage);
    void processUserMessage(_int code, UserMessage userMessage);
    void processFilesMessage(_int code, FilesMessage filesMessage);
};


#endif //VIRGILIUM_SERVER_H
