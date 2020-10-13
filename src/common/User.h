//
// Created by pinoOgni on 26/09/20.
//

#ifndef VIRGILIUM_USER_H
#define VIRGILIUM_USER_H

#include <string>
#include <QObject>
#include <QDataStream>
#include <iostream>
#include <QtGui/QColor>
#include "constants.h"

class User {
    QString email;
    QString password;
    QString firstName;
    QString lastName;
    _int siteId;
    _int lastCursorPos;
    QColor assignedColor;

public:
    User(QString email, QString password, QString firstName, QString lastName);

    User(QString email, QString password);

    User(QString email);

    User();

    void writeStream(QDataStream &out);

    friend QDataStream &operator<<(QDataStream &stream, const User &myClass);

    friend QDataStream &operator>>(QDataStream &stream, User &myClass);

    bool operator==(const User &other);

    bool operator<(const User &other) const;

    QString printMessage();

    QString getEmail();

    QString getPassword();

    QString getFirstName() const;

    QString getLastName() const;

    _int getSiteId() const;

    _int getLastCursorPos() const;

    QColor getAssignedColor() const;

    void setSiteId(_int siteId);

    void setLastCursorPos(_int pos);

    void setAssignedColor(QColor color);
};

#endif //VIRGILIUM_USER_H
