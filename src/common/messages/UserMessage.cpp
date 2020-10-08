//
// Created by alex on 23/09/20.
//

#include "UserMessage.h"

#include <utility>

UserMessage::UserMessage() = default;

UserMessage::UserMessage(qintptr sender, User user) : BasicMessage(sender), user(std::move(user)) {}

UserMessage::UserMessage(qintptr sender, User user, QString fileName) : BasicMessage(sender), user(std::move(user)),
                                                                        fileName(fileName) {}

User UserMessage::getUser() {
    return this->user;
}

QString UserMessage::getFileName() {
    return this->fileName;
}


//scrive sullo stream (this client è il mittente)
QDataStream &operator<<(QDataStream &stream, const UserMessage &myclass) {
    stream << myclass.user;
    stream << myclass.fileName;
    return stream;
}
//legge dallo stream (this client è il ricevente)
QDataStream &operator>>(QDataStream &stream, UserMessage &myclass) {
    stream >> myclass.user;
    stream >> myclass.fileName;
    return stream;
}

