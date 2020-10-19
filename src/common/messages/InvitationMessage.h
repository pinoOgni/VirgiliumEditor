
//
// Created by pinoOgni on 29/09/20.
//

#ifndef VIRGILIUM_INVITATIONMESSAGE_H
#define VIRGILIUM_INVITATIONMESSAGE_H


#include <string>
#include <QObject>
#include <QDataStream>
#include <QDebug>
#include "BasicMessage.h"
#include <iostream>

class InvitationMessage : public BasicMessage {
    QString email;
    QString invitationCode;

public:
    InvitationMessage();

    InvitationMessage(quintptr sender, QString email, QString invitationCode);

    friend QDataStream &operator<<(QDataStream &stream, const InvitationMessage &myclass);

    friend QDataStream &operator>>(QDataStream &stream, InvitationMessage &myclass);

    QString getEmail() const;

    QString getInvitationCode() const;

};


#endif //VIRGILIUM_INVITATIONMESSAGE_H
