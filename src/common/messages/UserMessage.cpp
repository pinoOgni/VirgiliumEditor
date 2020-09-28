//
// Created by alex on 23/09/20.
//

#include "UserMessage.h"

UserMessage::UserMessage(qintptr sender, User user): BasicMessage(sender), user(std::move(user)) {}

User UserMessage::getUser() {
    return this->user;
}

UserMessage::UserMessage() {}



//scrive sullo stream (this client è il mittente)
QDataStream &operator<<(QDataStream &stream, const UserMessage &myclass) {
    stream << myclass.user;
    return stream;
}
//legge dallo stream (this client è il ricevente)
QDataStream &operator>>(QDataStream &stream, UserMessage &myclass) {
    stream >> myclass.user;
    return stream;
}
