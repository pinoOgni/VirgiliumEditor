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


/*
quasi uguale a quella del lab3


*/
class message {
    
        symbol s; //il simbolo dentro di se ha un sacco di cose (come un vettore a lunghezza variabile: si sfruttano gli array di qt)
        _int from; //da chi è arrivato il messaggio? Ossia da che site_id?
        QString action; //dice quello che deve fare il client: insert, delete.....
    public:
        message();
        message(symbol s, _int from, QString action);
        _int getFrom() const;
        symbol getSymbol() const;
        QString getAction() const;

         void printMessage();

    //(https://www.html.it/pag/65301/funzioni-friend/)
    friend QDataStream &operator <<(QDataStream &stream, const message &myclass);
    friend QDataStream &operator >>(QDataStream &stream, message &myclass);
};
//static QDataStream &operator <<(QDataStream &stream, const message &myclass);
//static QDataStream &operator >>(QDataStream &stream, message &myclass);

#endif //VIRGILIUMCLIENT_MESSAGE_H
