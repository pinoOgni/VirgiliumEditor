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
   // QString print =  myclass.email + " " + myclass.password + " " + myclass.firstname + " " + myclass.lastname;
    //std::cout << print.toUtf8().constData();

    stream << myclass.email;
    stream << myclass.password;
    stream << myclass.firstname;
    stream << myclass.lastname;

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

bool User::operator==(User other) {
    return this->getEmail() == other.getEmail();
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

QString User::getFirstName() const {
    return this->firstname;
}

QString User::getLastName() const {
    return this->lastname;
}

_int User::getSiteId() {
    return this->siteId;
}

_int User::getLastCursorPos() {
    return this->lastCursorPos;
}

QColor User::getAssignedColor() const {
    return this->assignedColor;
}

void User::setSiteId(_int id) {
    this->siteId = id;
}

void User::setLastCursorPos(_int pos) {
    this->lastCursorPos = pos;
}

void User::setAssignedColor(QColor color) {
    this->assignedColor = std::move(color);
}

