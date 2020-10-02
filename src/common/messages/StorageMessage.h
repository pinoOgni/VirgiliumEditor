//
// Created by simod on 28/09/2020.
//

#ifndef VIRGILIUM_STORAGEMESSAGE_H
#define VIRGILIUM_STORAGEMESSAGE_H

#include <QtCore/QString>
#include <common/CRDT/Symbol.h>
#include <common/User.h>
#include "BasicMessage.h"

class StorageMessage : public BasicMessage {
    QString fileName;
    QVector<Symbol> _symbols;
    QList<User> activeUsers;

public:
    StorageMessage(qintptr sender, QVector<Symbol> _symbols, QString fileName, QList<User> users);

    explicit StorageMessage();

    QString getFileName();

    QVector<Symbol> getSymbols();

    QList<User> getActiveUsers();

    friend QDataStream &operator<<(QDataStream &stream, const StorageMessage &myClass);

    friend QDataStream &operator>>(QDataStream &stream, StorageMessage &myClass);
};


#endif //VIRGILIUM_STORAGEMESSAGE_H
