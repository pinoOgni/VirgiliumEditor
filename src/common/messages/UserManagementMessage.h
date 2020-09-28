//
// Created by pinoOgni on 26/09/20.
//

#ifndef VIRGILIUM_USERMANAGEMENTMESSAGE_H
#define VIRGILIUM_USERMANAGEMENTMESSAGE_H
#include <string>
#include <QObject>
#include <QDataStream>
#include "BasicMessage.h"

class UserManagementMessage: public BasicMessage {
    QString email_collaborator;
    QString email_owner;
    QString filename;
    QString password;

public:
    UserManagementMessage();
    //add_collaborator, remove_collaborator, unsubscribe
    UserManagementMessage(quintptr sender,QString email_collaborator, QString email_owner, QString filename, QString password);

    friend QDataStream &operator <<(QDataStream &stream, const UserManagementMessage &myclass);
    friend QDataStream &operator >>(QDataStream &stream, UserManagementMessage &myclass);

    QString getEmail_collaborator() const;
    QString getEmail_owner() const;
    QString getFilename() const;
    QString getPassword() const;
};




#endif //VIRGILIUM_USERMANAGEMENTMESSAGE_H
