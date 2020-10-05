//
// Created by alex on 10/08/20.
//

#ifndef VIRGILIUM_CRDTMESSAGE_H
#define VIRGILIUM_CRDTMESSAGE_H

#include <common/CRDT/Symbol.h>
#include "BasicMessage.h"

class CrdtMessage : public BasicMessage {
    Symbol s;
    QString action;
    bool mode;
    QString fileName;
public:
    CrdtMessage();

    CrdtMessage(quintptr sender, Symbol s, boolean mode, QString action, QString fileName);

    Symbol getSymbol() const;

    QString getAction() const;

    boolean getMode() const;

    void setMode(boolean mode);

    QString getFileName() const;

    void printMessage();

    friend QDataStream &operator<<(QDataStream &stream, const CrdtMessage &myclass);

    friend QDataStream &operator>>(QDataStream &stream, CrdtMessage &myclass);
};

#endif //VIRGILIUM_CRDTMESSAGE_H
