//
// Created by pinoOgni on 26/09/20.
//

#ifndef VIRGILIUM_FILESMESSAGE_H
#define VIRGILIUM_FILESMESSAGE_H
#include <string>
#include <QObject>
#include <QDataStream>
#include <QDebug>
#include "BasicMessage.h"
#include <iostream>


class FilesMessage: public BasicMessage{
    QString filename;
    QString last_access;
    QStringList collaborators;
    QString owner;

public:
    FilesMessage();
    FilesMessage(quintptr sender,QString filename, QString last_access, QStringList collaborators, QString owner);
    void writeStream(QDataStream& out);
    friend QDataStream &operator <<(QDataStream &stream, const FilesMessage &myclass);
    friend QDataStream &operator >>(QDataStream &stream, FilesMessage &myclass);
    void printUserInfo();
    QString getFilename() const;
    QString getLast_access() const;
    QStringList getCollaborators() const;
    QString getOwner() const;

};


#endif //VIRGILIUM_FILESMESSAGE_H
