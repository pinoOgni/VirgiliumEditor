#ifndef VIRGILIUM_SYMBOL_H
#define VIRGILIUM_SYMBOL_H

#include <QObject>
#include <QtCore/QString>
#include <common/constants.h>
#include <QtCore/QVector>
#include <QColor>
#include <QtCore/QTextStream>

/*
 * A symbol contains several fields: 'letter' specifies the letter to be inserted or deleted in the document, '_siteID'
 * is the identifier of the client to which the letter is associated, '_counterID' is a progressive number associated
 * with the letter and assigned by the client, 'pos' is the fractional position of the letter within the document.
 */
class Symbol {
    QString letter;
    _int _siteId;
    _int _counterId;
    QVector<_int> pos;
public:

    /*
     * The 'CharFormat' structure consists of two fields, the first specifies the color of the letter, the second the font
     */
    struct CharFormat {
        QColor foreground;
        QString font;
    } font;

    Symbol();

    Symbol(QString letter, _int _siteId, _int _counterId, QVector<_int> pos, const CharFormat &font);

    QString getLetter() const;

    _int getSiteId() const;

    _int getCounterId() const;

    CharFormat getFont() const;

    QVector<_int> getPosition() const;

    /*
     * override of the '==' operator for an object of type symbol
     */
    bool operator==(const Symbol &b);

    /*
     * Override of the '<=' operator for an object of type symbol
     * It is used within the clientProcess() to identify the correct fractional position within the document,
     * in which to insert the symbol
     */
    bool operator<=(const Symbol &b);

    /*
     * it is used to write a symbol in a data stream
     */
    friend QDataStream &operator<<(QDataStream &stream, const Symbol &myClass);

    /*
     * it is used to read a symbol in a data stream
     */
    friend QDataStream &operator>>(QDataStream &stream, Symbol &myClass);
};

#endif //VIRGILIUM_SYMBOL_H
