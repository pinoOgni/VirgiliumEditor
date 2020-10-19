//
// Created by alex on 10/08/20.
//

#include "BasicMessage.h"
#include <QDataStream>

BasicMessage::BasicMessage(_int sender) : sender(sender) {

}

QDataStream &operator<<(QDataStream &stream, const BasicMessage &myclass) {
    stream << myclass.sender;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, BasicMessage &myclass) {
    stream >> myclass.sender;
    return stream;
}

BasicMessage::BasicMessage() : sender(-1) {

}

_int BasicMessage::getSender() const {
    return this->sender;
}

void BasicMessage::setSender(_int sender) {
    BasicMessage::sender = sender;
}
