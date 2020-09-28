//
// Created by pinoOgni on 26/09/20.
//

#ifndef VIRGILIUM_USER_H
#define VIRGILIUM_USER_H
#include <string>
#include <QObject>
#include <QDataStream>
#include <iostream>
#include "constants.h"

class User {

    QString email;
    QString password;
    QString firstname;
    QString lastname;

public:
    User();
    User(QString email, QString password, QString firstname, QString lastname);
    User(QString email, QString password);
    User(QString email);
    void writeStream(QDataStream& out);
    friend QDataStream &operator <<(QDataStream &stream, const User &myclass);
    friend QDataStream &operator >>(QDataStream &stream, User &myclass);
    QString printMessage();

    QString getEmail();
    QString getPassword();
    QString getFirstName();
    QString getLastName();
};

#endif //VIRGILIUM_USER_H
