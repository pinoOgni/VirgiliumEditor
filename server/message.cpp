//
// Created by alex on 12/12/19.
//

#include <iostream>
#include "message.h"

message::message(symbol s, _int from, QString action): s(s),from(from),action(action){};
_int  message::getFrom() const{

    return this->from;
}

QString message::getAction() const {
    return this->action;
}

symbol message::getSymbol() const {
    return this->s;
}

void message::writeStream(QDataStream& out) {
    this->writeStream(out);
    out<<this->action;
    out<<this->from;
}

QDataStream &operator<<(QDataStream &stream, const message &myclass) {
    stream << myclass.s;
    stream << myclass.from;
    stream << myclass.action;
    //stream << myclass.letter;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, message &myclass) {
    stream >> myclass.s;
    stream >> myclass.from;
    stream >> myclass.action;
    return stream;
}

message::message() {

}

void message::printMessage() {
    std::cout<<"messaggio inviato da: "<<this->from<<"action=\""<<this->action.toStdString()<<"\""<<std::endl;
    std::cout<<"symbol:\n";
    this->s.printSymbol();

}

