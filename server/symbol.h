//
// Created by alex on 12/12/19.
//

#ifndef VIRGILIUMCLIENT_SYMBOL_H
#define VIRGILIUMCLIENT_SYMBOL_H


#include <vector>
#include "constants.h"
#include <QObject>
#include <QtCore/QVector>

class symbol {
    QString letter;
    _int _siteId;
    _int _counterId;
    QVector<_int> pos;
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
