//
// Created by simod on 28/09/2020.
//

#include "StorageMessage.h"

#include <utility>
#include <iostream>

StorageMessage::StorageMessage(qintptr sender, QVector<Symbol> _symbols, QString fileName): BasicMessage(sender), _symbols(std::move(_symbols)), fileName(std::move(fileName)) {}

StorageMessage::StorageMessage() = default;

QVector<Symbol> StorageMessage::getSymbols() {
    return this->_symbols;
}

QString StorageMessage::getFileName() {
    return this->fileName;
}

/* write on stream */
QDataStream &operator<<(QDataStream &stream, const StorageMessage &myClass) {
    stream << myClass.fileName;


    for(const Symbol& _symbol : myClass._symbols)
        stream << _symbol;
    return stream;
}

/* read from stream */
QDataStream &operator>>(QDataStream &stream, StorageMessage &myClass) {
    stream >> myClass.fileName;
    for(Symbol& _symbol : myClass._symbols)
        stream >> _symbol;
    return stream;
}
