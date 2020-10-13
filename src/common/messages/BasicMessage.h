//
// Created by alex on 10/08/20.
//

#ifndef VIRGILIUM_BASICMESSAGE_H
#define VIRGILIUM_BASICMESSAGE_H

#include <common/constants.h>
#include <QtGui/qopengl.h>

class BasicMessage {
protected:
    _int sender;
public:
    void setSender(_int sender);

public:
    explicit BasicMessage(_int sender);

    BasicMessage();

    friend QDataStream &operator<<(QDataStream &stream, const BasicMessage &myclass);

    friend QDataStream &operator>>(QDataStream &stream, BasicMessage &myclass);

    _int getSender() const;

};


#endif //VIRGILIUM_BASICMESSAGE_H
