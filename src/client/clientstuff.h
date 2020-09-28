//
// Created by pinoOgni on 10/08/20.
//

#ifndef VIRGILIUM_CLIENTSTUFF_H
#define VIRGILIUM_CLIENTSTUFF_H

#include <QString>
#include "../common/messages/FilesMessage.h"
#include "../common/messages/UserMessage.h"
#include "../common/constants.h"
#include "../common/messages/FileManagementMessage.h"
#include "../common/messages/UserManagementMessage.h"
#include "../common/messages/ChangePasswordMessage.h"
#include "../common/ClientSocket.h"


class ClientStuff : public QObject
{
    Q_OBJECT

public:
    ClientStuff(const QString hostAddress, int portVal, QObject *parent = 0);
    ~ClientStuff();
    ClientSocket *clientSocket;
    ClientSocket *getSocket();

public slots:
    void processBasicMessage(_int, BasicMessage);
    void processLoginAndSignup(_int);
    void processUserMessage(_int,UserMessage);
    void processFilesMessage(_int,std::vector<FilesMessage>);
    void processAllData(_int code, UserMessage userMessage, _int row1, std::vector<FilesMessage> filesOwner, _int row2, std::vector<FilesMessage> filesCollabs);
    void processFileManagement(_int);
    void processChangePassword(_int);
    void processUserManagement(_int);
signals:
    void hasReadSome(QString msg);
    void tryToLogin(bool logged);
    void tryToSignup(bool registered);

    void getFilesOwner(int row,std::vector<FilesMessage>& filesMessage);
    void getInfoUser(UserMessage &u);
    void getUserFiles(int row,std::vector<FilesMessage>& filesMessage);
    void isFileRenamed(bool);
    void isFileDeleted(bool);
    void isPswChanged(bool);
    void isFileCreated(bool);
    void isCollaboratorAdded(bool);
    void isCollaboratorRemoved(bool);
    void isUnsubscribed(bool);
    void getAllData(UserMessage &, _int ,std::vector<FilesMessage>&,_int,std::vector<FilesMessage>&);


private slots:

private:
    QString host;
    int port;
    bool status;

};

#endif // VIRGILIUM_CLIENTSTUFF_H
