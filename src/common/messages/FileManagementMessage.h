//
// Created by pinoOgni on 26/09/20.
//

#ifndef VIRGILIUM_FILEMANAGEMENTMESSAGE_H
#define VIRGILIUM_FILEMANAGEMENTMESSAGE_H
#include <string>
#include <QObject>
#include <QDataStream>
#include "BasicMessage.h"

class FileManagementMessage: public BasicMessage {
    QString email;
    QString filename;
    QString oldfilename;
    QString password;

public:
    FileManagementMessage();
    FileManagementMessage(quintptr sender,QString email, QString filename, QString oldfilename, QString password); //rename_file
    FileManagementMessage(quintptr sender,QString email, QString filename, QString password);  //new_file and delete_file

    friend QDataStream &operator <<(QDataStream &stream, const FileManagementMessage &myclass);
    friend QDataStream &operator >>(QDataStream &stream, FileManagementMessage &myclass);

    QString getEmail() const;
    QString getFilename() const;
    QString getOldfilename() const;
    QString getPassword() const;
};


#endif //VIRGILIUM_FILEMANAGEMENTMESSAGE_H
