//
// Created by pinoOgni on 26/09/20.
//

#include "User.h"
#include <utility>

User::User(QString email, QString password, QString firstName, QString lastName) :
        email(std::move(email)), password(std::move(password)), firstName(std::move(firstName)),
        lastName(std::move(lastName)) {};

User::User(QString email, QString password) :
        email(std::move(email)), password(std::move(password)) {};

User::User(QString email) : email(std::move(email)) {};

User::User() {};

// write on stream (this client è il mittente)
QDataStream &operator<<(QDataStream &stream, const User &myclass) {
    stream << myclass.email;
    stream << myclass.password;
    stream << myclass.firstName;
    stream << myclass.lastName;
    stream << myclass.siteId;
    stream << myclass.lastCursorPos;
    stream << myclass.assignedColor;

    return stream;
}

// read from stream (this client è il ricevente)
QDataStream &operator>>(QDataStream &stream, User &myclass) {
    stream >> myclass.email;
    stream >> myclass.password;
    stream >> myclass.firstName;
    stream >> myclass.lastName;
    stream >> myclass.siteId;
    stream >> myclass.lastCursorPos;
    stream >> myclass.assignedColor;

    return stream;
}

bool User::operator==(const User &other) {
    return this->email == other.email;
}

bool User::operator<(const User &other) const {
    return this->siteId < other.siteId;
}

QString User::printMessage() {
    return this->firstName + " " + this->lastName + " " + this->email + " " + this->password;
}

QString User::getEmail() {
    return this->email;
}

QString User::getPassword() {
    return this->password;
}

QString User::getFirstName() const {
    return this->firstName;
}

QString User::getLastName() const {
    return this->lastName;
}

_int User::getSiteId() const {
    return this->siteId;
}

_int User::getLastCursorPos() const {
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
