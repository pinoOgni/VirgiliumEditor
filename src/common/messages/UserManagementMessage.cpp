//
// Created by pinoOgni on 26/09/20.
//

#include "UserManagementMessage.h"

#include <utility>

UserManagementMessage::UserManagementMessage() {};

UserManagementMessage::UserManagementMessage(
        quintptr sender,
        QString email_collaborator,
        QString email_owner,
        QString filename,
        QString password) :
        BasicMessage(sender),
        email_collaborator(std::move(email_collaborator)),
        email_owner(std::move(email_owner)),
        filename(std::move(filename)),
        password(std::move(password)) {};

QString UserManagementMessage::getEmail_collaborator() const { return email_collaborator; }

QString UserManagementMessage::getEmail_owner() const { return email_owner; }

QString UserManagementMessage::getFilename() const { return filename; }

QString UserManagementMessage::getPassword() const { return password; }

QDataStream &operator<<(QDataStream &stream, const UserManagementMessage &myclass) {
    stream << myclass.email_collaborator;
    stream << myclass.email_owner;
    stream << myclass.filename;
    stream << myclass.password;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, UserManagementMessage &myclass) {
    stream >> myclass.email_collaborator;
    stream >> myclass.email_owner;
    stream >> myclass.filename;
    stream >> myclass.password;
    return stream;

}


