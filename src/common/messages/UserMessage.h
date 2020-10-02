//
// Created by alex on 23/09/20.
//

#ifndef VIRGILIUM_USERMESSAGE_H
#define VIRGILIUM_USERMESSAGE_H

#include <common/User.h>
#include "BasicMessage.h"

class UserMessage: public BasicMessage {
    User user;
    QString fileName;
public:
    UserMessage(qintptr sender, User user);

    UserMessage(qintptr sender, User user, QString fileName);

    explicit UserMessage();

    User getUser();

    QString getFileName();

    friend QDataStream &operator <<(QDataStream &stream, const UserMessage &myclass);
    friend QDataStream &operator >>(QDataStream &stream, UserMessage &myclass);

};


#endif //VIRGILIUM_USERMESSAGE_H
