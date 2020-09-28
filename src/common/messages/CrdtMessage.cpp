//
// Created by alex on 10/08/20.
//

#include <iostream>
#include "CrdtMessage.h"
#include <QDataStream>
CrdtMessage::CrdtMessage():BasicMessage(sender) {

}

CrdtMessage::CrdtMessage(quintptr sender, Symbol s, _int from, QString action):
    BasicMessage(sender),
    s(s),
    from(from),
    action(action) {

}

_int CrdtMessage::getFrom() const {
    return this->from;
    return 0;
}

Symbol CrdtMessage::getSymbol() const {
    return this->s;
}

QString CrdtMessage::getAction() const {
    return this->action;
}


void CrdtMessage::printMessage() {
    std::cout<<"messaggio inviato da: "<<this->from<<"action=\""<<this->action.toStdString()<<"\""<<std::endl;
    std::cout<<"symbol:\n";
    this->s.printSymbol();
}

QDataStream &operator<<(QDataStream &stream, const CrdtMessage &myclass) {
    stream << myclass.sender;
    stream << myclass.s;
    stream << myclass.from;
    stream << myclass.action;
    //stream << myclass.letter;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, CrdtMessage &myclass) {
    stream >> myclass.sender;
    stream >> myclass.s;
    stream >> myclass.from;
    stream >> myclass.action;
    return stream;
}