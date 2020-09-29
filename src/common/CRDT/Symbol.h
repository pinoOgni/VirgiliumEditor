//
// Created by alex on 10/08/20.
//

#ifndef VIRGILIUM_SYMBOL_H
#define VIRGILIUM_SYMBOL_H

#include <QObject>
#include <QtCore/QString>
#include <common/constants.h>
#include <QtCore/QVector>
#include <QColor>
#include <QtCore/QTextStream>

class Symbol {
    QString letter;
    _int _siteId;
    _int _counterId;
    QVector<_int> pos;
public:
    struct CharFormat {
        QColor foreground;
        QString font;
    } font;

    Symbol();
    Symbol(QString letter, _int _siteId,_int _counterId, QVector<_int> pos,CharFormat font);
    QString getLetter() const;
    _int getSiteId() const;
    _int getCounterId() const;
    QVector<_int> getPosition() const;
    bool operator==(const Symbol& b);
    bool operator<(const Symbol& b);
    bool operator<=(const Symbol& b);
    Symbol& operator=(const Symbol& other);
    void writeStream(QTextStream& out);
    CharFormat getFont() const;
    void printSymbol();

    friend QDataStream &operator <<(QDataStream &stream, const Symbol &myClass);
    friend QDataStream &operator >>(QDataStream &stream, Symbol &myClass);
};


#endif //VIRGILIUM_SYMBOL_H
