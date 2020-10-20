//
// Created by alex on 07/12/19.
//

#include "Symbol.h"
#include <QDataStream>
#include <utility>

Symbol::Symbol() {}

Symbol::Symbol(QString letter, _int _siteId, _int _counterId, QVector<_int> pos, const CharFormat &font) :
        letter(std::move(letter)), _siteId(_siteId), _counterId(_counterId), pos(std::move(pos)) {
    this->font.font = font.font;
    this->font.foreground = font.foreground;
}

QString Symbol::getLetter() const {
    return this->letter;
}

_int Symbol::getSiteId() const {
    return this->_siteId;
}

_int Symbol::getCounterId() const {
    return this->_counterId;
}

Symbol::CharFormat Symbol::getFont() const {
    return this->font;
}

QVector<_int> Symbol::getPosition() const {
    return this->pos;
}

bool Symbol::operator==(const Symbol &b) {
    return (this->_siteId == b._siteId && this->_counterId == b._counterId && this->letter == b.letter &&
            this->pos == b.pos);
}

bool Symbol::operator<=(const Symbol &b) {
    if (this->pos == b.pos) {
        return this->_siteId < b._siteId;
    }

    _int i;
    bool flag = false;

    _int _max = (this->pos.size() < b.pos.size()) ? this->pos.size() : b.pos.size();

    for (i = 0; i < _max; i++) {
        if (this->pos[i] < b.pos[i]) {
            flag = true;
            break;
        } else {
            if (this->pos[i] > b.pos[i]) {
                flag = false;
                break;
            }
        }
    }
    return flag;
}

QDataStream &operator<<(QDataStream &stream, const Symbol &myClass) {
    stream << myClass._siteId;
    stream << myClass._counterId;
    stream << myClass.pos;
    stream << myClass.letter;
    stream << myClass.font.font;
    stream << myClass.font.foreground;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Symbol &myClass) {
    stream >> myClass._siteId;
    stream >> myClass._counterId;
    stream >> myClass.pos;
    stream >> myClass.letter;
    stream >> myClass.font.font;
    stream >> myClass.font.foreground;
    return stream;
}



