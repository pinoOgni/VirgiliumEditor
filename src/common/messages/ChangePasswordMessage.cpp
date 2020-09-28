//
// Created by pinoOgni on 26/09/20.
//

#include "ChangePasswordMessage.h"

#include <utility>

ChangePasswordMessage::ChangePasswordMessage() {};
ChangePasswordMessage::ChangePasswordMessage(
        quintptr sender,
        QString email,
        QString oldPsw,
        QString newPsw):
        BasicMessage(sender),
        email(std::move(email)),
        oldPsw(std::move(oldPsw)),
        newPsw(std::move(newPsw)) {};


QString ChangePasswordMessage::getEmail() const { return email; }
QString ChangePasswordMessage::getOldPsw() const { return oldPsw; }
QString ChangePasswordMessage::getNewPsw() const { return newPsw; }


QDataStream &operator <<(QDataStream &stream, const ChangePasswordMessage &myclass) {
    stream << myclass.email;
    stream << myclass.oldPsw;
    stream << myclass.newPsw;
    return stream;
}

QDataStream &operator >>(QDataStream &stream, ChangePasswordMessage &myclass) {
    stream >> myclass.email;
    stream >> myclass.oldPsw;
    stream >> myclass.newPsw;
    return stream;
}