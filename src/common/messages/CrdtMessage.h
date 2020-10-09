//
// Created by alex on 10/08/20.
//

#ifndef VIRGILIUM_CRDTMESSAGE_H
#define VIRGILIUM_CRDTMESSAGE_H

#include <common/CRDT/Symbol.h>
#include "BasicMessage.h"

class CrdtMessage : public BasicMessage {
    QVector<Symbol> symbols;
    //Symbol s;
    QString action;
    bool mode;
    QString fileName;
public:
    CrdtMessage();

    CrdtMessage(quintptr sender, QVector<Symbol> symbols, bool mode, QString action, QString fileName);

    QVector<Symbol> getSymbols() const;

    QString getAction() const;

    bool getMode() const;

    void setMode(bool mode);

    QString getFileName() const;

    friend QDataStream &operator<<(QDataStream &stream, const CrdtMessage &myclass);

    friend QDataStream &operator>>(QDataStream &stream, CrdtMessage &myclass);
};

#endif //VIRGILIUM_CRDTMESSAGE_H
