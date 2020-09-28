//
// Created by pinoOgni on 26/09/20.
//

#include "User.h"

#include <utility>
User::User(
        QString email,
        QString password,
        QString firstname,
        QString lastname):
        email(std::move(email)),
        password(std::move(password)),
        firstname(std::move(firstname)),
        lastname(std::move(lastname)){};

User::User(
        QString email,
        QString password):
        email(std::move(email)),
        password(std::move(password)) {};


User::User(
        QString email):
        email(std::move(email)) {};




//scrive sullo stream (this client è il mittente)
QDataStream &operator<<(QDataStream &stream, const User &myclass) {
    QString print =  myclass.email + " " + myclass.password + " " + myclass.firstname + " " + myclass.lastname;
    std::cout << print.toUtf8().constData();

    stream << myclass.email;
    stream << myclass.password;
    if(!myclass.firstname.isEmpty()) {
        stream << myclass.firstname;
        stream << myclass.lastname;
    }
    return stream;
}
//legge dallo stream (this client è il ricevente)
QDataStream &operator>>(QDataStream &stream, User &myclass) {
    stream >> myclass.email;
    stream >> myclass.password;
    stream >> myclass.firstname;
    stream >> myclass.lastname;
    return stream;
}

User::User() {}

QString User::printMessage() {
    return this->firstname + " " + this->lastname + " " + this->email + " " + this->password;
}

QString User::getEmail() {
    return this->email;
}
QString User::getPassword() {
    return this->password;
}
QString User::getFirstName() {
    return this->firstname;
}
QString User::getLastName() {
    return this->lastname;
}

