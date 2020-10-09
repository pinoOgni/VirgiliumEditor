//
// Created by alex on 10/08/20.
//

#include <iostream>
#include "CrdtMessage.h"
#include <QDataStream>
#include <utility>

CrdtMessage::CrdtMessage() : BasicMessage(sender) {}

CrdtMessage::CrdtMessage(quintptr sender, QVector<Symbol> symbols, bool mode, QString action, QString fileName) :
        BasicMessage(sender),
        symbols(std::move(symbols)),
        mode(mode),
        fileName(std::move(fileName)),
        action(std::move(action)) {}

QVector<Symbol> CrdtMessage::getSymbols() const {
    return this->symbols;
}

QString CrdtMessage::getAction() const {
    return this->action;
}

bool CrdtMessage::getMode() const {
    return this->mode;
}

void CrdtMessage::setMode(bool m) {
    this->mode = m;
}

QString CrdtMessage::getFileName() const {
    return this->fileName;
}

QDataStream &operator<<(QDataStream &stream, const CrdtMessage &myClass) {
    stream << myClass.sender;
    stream << myClass.symbols;
    stream << myClass.mode;
    stream << myClass.action;
    stream << myClass.fileName;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, CrdtMessage &myclass) {
    stream >> myclass.sender;
    stream >> myclass.symbols;
    stream >> myclass.mode;
    stream >> myclass.action;
    stream >> myclass.fileName;
    return stream;
}