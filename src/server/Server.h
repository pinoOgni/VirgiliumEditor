#ifndef VIRGILIUM_SERVER_H
#define VIRGILIUM_SERVER_H

#include <QtNetwork/QTcpServer>
#include <common/messages/UserMessage.h>
#include <server/model/Model.h>
#include "../common/messages/FilesMessage.h"
#include "../common/messages/CrdtMessage.h"

/*
 * The Server class extends the QTcpServer class which provides a TCP-based server.
 */

class Server : public QTcpServer {
Q_OBJECT

    Model &model;

public:
    Server(quint16 port, Model &model);

    ~Server();

    /*
     * This function is called by QTcpServer when a new connection is available.
     * The socketDescriptor argument is the native socket descriptor for the accepted connection.
     * The implementation creates a QTcpSocket and sets the socket descriptor; if it doesn't, deleteLater() function is called.
     * At the end a message with the identification code CLIENT_CONNECTED is sent to the data stream to notify the client
     * and populate the clientID field of the client side socket.
     */
    void incomingConnection(qintptr handle) override;

    static bool checkUpdate(const CrdtMessage &crdtMessage);

private slots:

    /*
     * Every time the socket state change stateChanged() signal is emitted and the onSocketStateChanged slot called.
     * This slot is used to intercept the ClosingState of the server side socket and to update the tables relating
     * to active and logged-in users in the event of unexpected closures.
     */
    void onSocketStateChanged(QTcpSocket::SocketState state);

    /*
     * The following slot are invoked every time a message is received from the client. There
     * is a different slot for each message managed inside the application and most of them
     * contains a switch - case statement in which different operations are performed.
     * */
    void onProcessCrdtMessage(_int code, const CrdtMessage &crdtMessage);

    void onProcessUserMessage(_int code, UserMessage userMessage);

    void onProcessStorageMessage(_int code, StorageMessage storageMessage);

    void onFileManagementMessageReceived(_int code, const FileManagementMessage &fileManagementMessage);

    void onChangePasswordMessageReceived(_int code, const ChangePasswordMessage &changePasswordMessage);

    void onUserManagementMessageReceived(_int code, const UserManagementMessage &userManagementMessage);

    void onInvitationReceived(_int code, InvitationMessage invitationMessage);

    void onLogoutReceived(_int code, UserMessage userMessage);

signals:

    void processBasicMessage(_int code, BasicMessage basicMessage);

    void processCrdtMessage(_int code, CrdtMessage crdtMessage);

    void processUserMessage(_int code, UserMessage userMessage);

    void processFilesMessage(_int code, FilesMessage filesMessage);
};


#endif //VIRGILIUM_SERVER_H
