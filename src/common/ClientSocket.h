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

/*
 * The ClientSocket class extends the QTcpSocket class which provides a TCP socket.
 * the class has two private fields: clientID is the identifier of the client to which the socket refers;
 * the value is that of the socketDescriptor assigned by the server.
 * The 'in' field specifies the data stream on which the data will be written and read
 */

class ClientSocket : public QTcpSocket {
Q_OBJECT
private:
    _int clientID;
    QDataStream in;
public:

    /* server side constructor */
    explicit ClientSocket(QObject *parent = nullptr);

    /* client side constructor */
    explicit ClientSocket(const QString &hostName, quint16 port, QObject *parent = nullptr);

    ~ClientSocket();

    _int getClientID();

    void setClientID(_int clientID);

    /*
    * override of the '==' operator. It is used to check the equality of the '_siteID' field assigned to the socket
    */
    bool operator==(const ClientSocket &b);

    bool operator==(const ClientSocket *b);

    /*
     * Communication between client and server takes place through the exchange of different types of messages.
     * Each of these messages is identified by an identification code followed by the actual message content.
     * The different send() methods write these types of messages to the data stream.
     */

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

private slots:

    /*
     * The readyRead() signal is emitted once every time new data is available for reading from the device's current read channel.
     * It will only be emitted again once new data is available, such as when a new payload of network data has arrived in the
     * network socket, or when a new block of data has been appended in the device.
     * When this happens the onReadyRead() slot is called.
     * The onReadyRead() slot first reads the code from the data stream and, on the basis of this, through a switch statement,
     * the rest of the message content.
     * Finally a further signal is emitted, which will be intercepted by the client or by the server depending on who it is intended for.
     */
    void onReadyRead();

    /*
     * The slot onDisconnectedSocketServer()is called when the signal disconnected() is emitted on the server side socket.
     * This slot call the deleteLater() function and free the associated socket through the destructor
     */
    void onDisconnectedSocketServer();

    /*
     * When the QTcpSocket error signal il emitted, the onDisplayError slot il called.
     * If an error occurs this is reported in the log file.
     */
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

    void logoutReceived(_int code, UserMessage userMessage); //pino close connection when client "logout"

    void crdtMessageReceived(_int code, CrdtMessage crdtMessage);

    void activeUserMessageReceived(_int code, ActiveUserMessage activeUserMessage);
};


#endif //VIRGILIUM_CLIENTSOCKET_H
