//
// Created by simod on 05/10/2020.
//

#include "ActiveUserMessage.h"

#include <utility>

ActiveUserMessage::ActiveUserMessage(qintptr sender, QList<User> activeUsers) : BasicMessage(sender),
                                                                                activeUsers(std::move(activeUsers)) {}

ActiveUserMessage::ActiveUserMessage() = default;

QList<User> ActiveUserMessage::getActiveUsers() {
    return this->activeUsers;
}

QDataStream &operator<<(QDataStream &stream, const ActiveUserMessage &myClass) {
    stream << myClass.activeUsers;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, ActiveUserMessage &myClass) {
    stream >> myClass.activeUsers;
    return stream;
}
