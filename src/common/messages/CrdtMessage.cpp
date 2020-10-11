//
// Created by alex on 10/08/20.
//

#include <iostream>
#include "CrdtMessage.h"
#include <QDataStream>
#include <utility>

CrdtMessage::CrdtMessage() : BasicMessage(sender) {}

CrdtMessage::CrdtMessage(quintptr sender, Symbol s, bool mode, QString action, QString fileName) :
        BasicMessage(sender),
        s(std::move(s)),
        mode(mode),
        fileName(fileName),
        action(std::move(action)) {}

Symbol CrdtMessage::getSymbol() const {
    return this->s;
}

QString CrdtMessage::getAction() const {
    return this->action;
}

bool CrdtMessage::getMode() const {
    return this->mode;
}

void CrdtMessage::setMode(bool mode) {
    this->mode = mode;
}

QString CrdtMessage::getFileName() const {
    return this->fileName;
}

void CrdtMessage::printMessage() {
    //std::cout<<"messaggio inviato da: "<<this->mode<<"action=\""<<this->action.toStdString()<<"\""<<std::endl;
    std::cout << "symbol:\n";
    this->s.printSymbol();
}

QDataStream &operator<<(QDataStream &stream, const CrdtMessage &myClass) {
    stream << myClass.sender;
    stream << myClass.s;
    stream << myClass.mode;
    stream << myClass.action;
    stream << myClass.fileName;
    //stream << myclass.letter;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, CrdtMessage &myclass) {
    stream >> myclass.sender;
    stream >> myclass.s;
    stream >> myclass.mode;
    stream >> myclass.action;
    stream >> myclass.fileName;
    return stream;
}