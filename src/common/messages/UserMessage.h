//
// Created by alex on 23/09/20.
//

#ifndef VIRGILIUM_USERMESSAGE_H
#define VIRGILIUM_USERMESSAGE_H

#include <common/User.h>
#include "BasicMessage.h"

class UserMessage: public BasicMessage {
    User user;
public:
    UserMessage(qintptr sender, User user);
    explicit UserMessage();
    User getUser();

    friend QDataStream &operator <<(QDataStream &stream, const UserMessage &myclass);
    friend QDataStream &operator >>(QDataStream &stream, UserMessage &myclass);

};


#endif //VIRGILIUM_USERMESSAGE_H
