//
// Created by alex on 12/12/19.
//

#ifndef VIRGILIUMCLIENT_SYMBOL_H
#define VIRGILIUMCLIENT_SYMBOL_H


#include <vector>
#include "constants.h"
#include <QObject>
#include <QtCore/QVector>
/*
il client possiede dentro di se un array di symbol (ossia il testo), sta anche dentro il message del client
site_id e client_id servono per garantire la commutatività e l'idempotenza (lab3) 
*/

class symbol {
    QString letter; //Qstring è meglio di char
    _int _siteId; //identifica il client che ha generato il symbol (message ha un site_id che identifica da quale client viene il messaggio)
    _int _counterId; //numero progressivo immesso dal client 
    QVector<_int> pos; //identifica il carattere all'interno del testo (algoritmo CRDT). Qvector sono già tarati per essere serializzati su uno stream
public:
    symbol();
    symbol(QString letter, _int _siteId,_int _counterId, QVector<_int> pos);
    QString getLetter() const;
    _int getSiteId() const;
    _int getCounterId() const;
    QVector<_int> getPosition() const;
    bool operator==(const symbol& b);
    bool operator<(const symbol& b);
    void writeStream(QDataStream& out);

    void printSymbol();

    friend QDataStream &operator <<(QDataStream &stream, const symbol &myclass);
    friend QDataStream &operator >>(QDataStream &stream, symbol &myclass);
};


#endif //VIRGILIUMCLIENT_SYMBOL_H
