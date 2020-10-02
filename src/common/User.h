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
    QString firstname;
    QString lastname;
    _int siteId;
    _int lastCursorPos;
    QColor assignedColor;

public:
    User();

    User(QString email, QString password, QString firstname, QString lastname);

    User(QString email, QString password);

    User(QString email);

    void writeStream(QDataStream &out);

    friend QDataStream &operator<<(QDataStream &stream, const User &myclass);

    friend QDataStream &operator>>(QDataStream &stream, User &myclass);

    bool operator==(User other);

    QString printMessage();

    QString getEmail();

    QString getPassword();

    QString getFirstName() const;

    QString getLastName() const;

    _int getSiteId();

    _int getLastCursorPos();

    QColor getAssignedColor() const;

    void setSiteId(_int siteId);

    void setLastCursorPos(_int pos);

    void setAssignedColor(QColor color);
};

#endif //VIRGILIUM_USER_H
