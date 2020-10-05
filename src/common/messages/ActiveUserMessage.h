//
// Created by simod on 05/10/2020.
//

#ifndef VIRGILIUM_ACTIVEUSERMESSAGE_H
#define VIRGILIUM_ACTIVEUSERMESSAGE_H


#include <common/User.h>
#include "BasicMessage.h"

class ActiveUserMessage : public BasicMessage {
    QList<User> activeUsers;

public:
    ActiveUserMessage(qintptr sender, QList<User> activeUsers);

    ActiveUserMessage();

    QList<User> getActiveUsers();

    friend QDataStream &operator<<(QDataStream &stream, const ActiveUserMessage &myClass);

    friend QDataStream &operator>>(QDataStream &stream, ActiveUserMessage &myClass);
};


#endif //VIRGILIUM_ACTIVEUSERMESSAGE_H
