//
// Created by pinoOgni on 29/09/20.
//

#include "InvitationMessage.h"
#include <utility>

InvitationMessage::InvitationMessage(
        quintptr sender,
        QString email,
        QString invitationCode):
        BasicMessage(sender),
        email(std::move(email)),
        invitationCode(std::move(invitationCode))
{}

QDataStream &operator <<(QDataStream &stream, const InvitationMessage &myclass) {
    stream << myclass.email;
    stream << myclass.invitationCode;
    return stream;
}
QDataStream &operator >>(QDataStream &stream, InvitationMessage &myclass) {
    stream >> myclass.email;
    stream >> myclass.invitationCode;
    return stream;
}


QString InvitationMessage::getEmail() const {    return email; }


QString InvitationMessage::getInvitationCode() const { return invitationCode; }

InvitationMessage::InvitationMessage() {}
