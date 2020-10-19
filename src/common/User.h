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

/*
 * The user class is used in order to keep all data related to a single client, for example firstName and lastName
 */

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

    /* write on stream (this client is the sender) */
    friend QDataStream &operator<<(QDataStream &stream, const User &myClass);

    /* read from stream (this client is the receiver) */
    friend QDataStream &operator>>(QDataStream &stream, User &myClass);

    bool operator==(const User &other);

    bool operator<(const User &other) const;

    QString getEmail() const;

    QString getPassword() const;

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
