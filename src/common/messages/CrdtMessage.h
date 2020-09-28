//
// Created by alex on 10/08/20.
//

#ifndef VIRGILIUM_CRDTMESSAGE_H
#define VIRGILIUM_CRDTMESSAGE_H


#include <common/CRDT/Symbol.h>
#include "BasicMessage.h"

class CrdtMessage: public BasicMessage {


    Symbol s;
    _int from;
    QString action;
public:
    CrdtMessage();
    CrdtMessage(quintptr sender,Symbol s, _int from, QString action);
    _int getFrom() const;
    Symbol getSymbol() const;
    QString getAction() const;

    void printMessage();
    friend QDataStream &operator <<(QDataStream &stream, const CrdtMessage &myclass);
    friend QDataStream &operator >>(QDataStream &stream, CrdtMessage &myclass);
};


#endif //VIRGILIUM_CRDTMESSAGE_H
