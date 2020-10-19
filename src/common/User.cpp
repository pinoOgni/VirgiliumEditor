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

QDataStream &operator<<(QDataStream &stream, const User &myClass) {
    stream << myClass.email;
    stream << myClass.password;
    stream << myClass.firstName;
    stream << myClass.lastName;
    stream << myClass.siteId;
    stream << myClass.lastCursorPos;
    stream << myClass.assignedColor;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, User &myClass) {
    stream >> myClass.email;
    stream >> myClass.password;
    stream >> myClass.firstName;
    stream >> myClass.lastName;
    stream >> myClass.siteId;
    stream >> myClass.lastCursorPos;
    stream >> myClass.assignedColor;

    return stream;
}

bool User::operator==(const User &other) {
    return this->email == other.email;
}

bool User::operator<(const User &other) const {
    return this->siteId < other.siteId;
}

QString User::getEmail() const {
    return this->email;
}

QString User::getPassword() const {
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
