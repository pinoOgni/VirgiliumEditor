//
// Created by alex on 10/08/20.
//

#ifndef VIRGILIUM_BASICMESSAGE_H
#define VIRGILIUM_BASICMESSAGE_H

#include <common/constants.h>
#include <QtGui/qopengl.h>

class BasicMessage {
protected:
    qintptr sender;
public:
    void setSender(qintptr sender);

public:
     explicit BasicMessage(qintptr sender);
     BasicMessage();
    friend QDataStream &operator <<(QDataStream &stream, const BasicMessage &myclass);
    friend QDataStream &operator >>(QDataStream &stream, BasicMessage &myclass);
    qintptr getSender();

};


#endif //VIRGILIUM_BASICMESSAGE_H
