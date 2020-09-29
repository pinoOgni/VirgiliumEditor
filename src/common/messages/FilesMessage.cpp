//
// Created by pinoOgni on 26/09/20.
//

#include "FilesMessage.h"
#include "StorageMessage.h"

#include <iostream>
#include <utility>

FilesMessage::FilesMessage(
        quintptr sender,
        QString filename,
        QString last_access,
        QStringList collaborators,
        QString owner):
        BasicMessage(sender),
        filename(std::move(filename)),
        last_access(std::move(last_access)),
        collaborators(std::move(collaborators)),
        owner(std::move(owner))
        {}

QDataStream &operator <<(QDataStream &stream, const FilesMessage &myclass) {
    stream << myclass.filename;
    stream << myclass.last_access;
    stream << myclass.collaborators;
    stream << myclass.owner;
    return stream;
}
QDataStream &operator >>(QDataStream &stream, FilesMessage &myclass) {
    qDebug() << myclass.filename;
    stream >> myclass.filename;
    stream >> myclass.last_access;
    stream >> myclass.collaborators;
    stream >> myclass.owner;
    return stream;
}

void FilesMessage::printUserInfo() {
    qDebug() << this->filename << " " << this->last_access << " " << this->collaborators << " " << this->owner;
}

QString FilesMessage::getFilename() const {    return filename; }

QString FilesMessage::getLast_access() const {   return last_access; }

QStringList FilesMessage::getCollaborators() const {   return collaborators; }

QString FilesMessage::getOwner() const {   return owner; }

FilesMessage::FilesMessage() {}
