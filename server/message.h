//
// Created by alex on 12/12/19.
//

#ifndef VIRGILIUMCLIENT_MESSAGE_H
#define VIRGILIUMCLIENT_MESSAGE_H
#include "constants.h"
#include <string>
#include <QObject>
#include "symbol.h"
#include <QDataStream>

class message {

    symbol s;
    _int from;
    QString action;
public:
    message();
    message(symbol s, _int from, QString action);
    _int getFrom() const;
    symbol getSymbol() const;
    QString getAction() const;
    void writeStream(QDataStream& out);

    void printMessage();
    friend QDataStream &operator <<(QDataStream &stream, const message &myclass);
    friend QDataStream &operator >>(QDataStream &stream, message &myclass);
};


#endif //VIRGILIUMCLIENT_MESSAGE_H
