//
// Created by simod on 28/09/2020.
//

#include "StorageMessage.h"
#include <utility>
#include <QDataStream>

StorageMessage::StorageMessage(qintptr sender, QVector<Symbol> _symbols, QString fileName, QList<User> users) :
        BasicMessage(sender),
        _symbols(std::move(_symbols)),
        fileName(std::move(fileName)),
        activeUsers(std::move(users)) {}

StorageMessage::StorageMessage() = default;

QString StorageMessage::getFileName() {
    return this->fileName;
}

QVector<Symbol> StorageMessage::getSymbols() {
    return this->_symbols;
}

QList<User> StorageMessage::getActiveUsers() {
    return this->activeUsers;
}

/* write on stream */
QDataStream &operator<<(QDataStream &stream, const StorageMessage &myClass) {
    stream << myClass.fileName;
    stream << myClass._symbols;
    stream << myClass.activeUsers;
    return stream;
}

/* read from stream */
QDataStream &operator>>(QDataStream &stream, StorageMessage &myClass) {
    stream >> myClass.fileName;
    stream >> myClass._symbols;
    stream >> myClass.activeUsers;
    return stream;
}
