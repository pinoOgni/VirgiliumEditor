//
// Created by pinoOgni on 26/09/20.
//

#include "FileManagementMessage.h"

#include <utility>

FileManagementMessage::FileManagementMessage() {};
FileManagementMessage::FileManagementMessage(
        quintptr sender,
        QString email,
        QString filename,
        QString oldfilename,
        QString password):
        BasicMessage(sender),
        email(std::move(email)),
        filename(std::move(filename)),
        oldfilename(std::move(oldfilename)),
        password(password)
        {};
FileManagementMessage::FileManagementMessage(
        quintptr sender,
        QString email,
        QString filename,
        QString password):
        BasicMessage(sender),
        email(std::move(email)),
        filename(std::move(filename)),
        password(std::move(password))
        {};


QString FileManagementMessage::getEmail() const { return email; }

QString FileManagementMessage::getFilename() const { return filename; }

QString FileManagementMessage::getOldfilename() const { return oldfilename; }

QString FileManagementMessage::getPassword() const { return password; }


QDataStream &operator <<(QDataStream &stream, const FileManagementMessage &myclass) {
    stream << myclass.sender;
    stream << myclass.email;
    stream << myclass.filename;

    stream << myclass.password;
    if(!myclass.oldfilename.isEmpty())
        stream << myclass.oldfilename;
    else
        stream << "oldFilename";
    return stream;
}

QDataStream &operator >>(QDataStream &stream, FileManagementMessage &myclass) {
    stream >> myclass.sender;
    stream >> myclass.email;
    stream >> myclass.filename;
    stream >> myclass.password;

    //only for rename
    stream >> myclass.oldfilename;
    return stream;

}

