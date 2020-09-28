//
// Created by pinoOgni on 26/09/20.
//

#ifndef VIRGILIUM_CHANGEPASSWORDMESSAGE_H
#define VIRGILIUM_CHANGEPASSWORDMESSAGE_H


#include <string>
#include <QObject>
#include <QDataStream>
#include "BasicMessage.h"

class ChangePasswordMessage: public BasicMessage {
    QString email;
    QString oldPsw;
    QString newPsw;

public:
    ChangePasswordMessage();
    ChangePasswordMessage(quintptr sender,QString email, QString oldPsw, QString newPsw);

    friend QDataStream &operator <<(QDataStream &stream, const ChangePasswordMessage &myclass);
    friend QDataStream &operator >>(QDataStream &stream, ChangePasswordMessage &myclass);

    QString getEmail() const;
    QString getOldPsw() const;
    QString getNewPsw() const;
};


#endif //VIRGILIUM_CHANGEPASSWORDMESSAGE_H
