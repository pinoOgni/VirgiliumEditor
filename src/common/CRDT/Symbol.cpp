//
// Created by alex, simone on 10/08/20.
//

#include <iostream>
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

//override dellìoperatore == per un oggetto di tipo symbol sennò il compilatore ti dice "che succede coglione?"
bool Symbol::operator==(const Symbol &b) {
    return (this->_siteId == b._siteId && this->_counterId == b._counterId && this->letter == b.letter &&
            this->pos == b.pos);
}

//stessa cosa di prima solo per l'operatore "<"
//serve perchè quando arriva una insert di un carattere da un altro client e devo mettere il carattere nella posizione giusta del testo di questo (this) client
/*bool Symbol::operator<(const Symbol &b) {
    _int i;
    bool flag = false;
    //devo ritornare se this è  (strettamente) minore di b
    _int _max = (this->pos.size() < b.pos.size()) ? this->pos.size() : b.pos.size();
    //ciclo sul vettore più corto

    for (i = 0; i < _max; i++) {
        if (this->pos[i] < b.pos[i])
            flag = true;
    }
    if (i == _max && b.pos.size() > this->pos.size())
        flag = true;
    return flag;
}*/

bool Symbol::operator<=(const Symbol &b) {
    if (this->pos == b.pos) {
        return this->_siteId < b._siteId;
    }

    _int i;
    bool flag = false;
    //devo ritornare se this è  (strettamente) minore di b
    _int _max = (this->pos.size() < b.pos.size()) ? this->pos.size() : b.pos.size();
    //ciclo sul vettore più corto

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
    /*if(i==_max&&b.pos.size()>this->pos.size())
        flag=true;*/
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

//TODO: da cancellare? per debug
void Symbol::printSymbol() {
    std::string out = "valore :";
    //QChar::to
    std::cout << "valore: " << this->letter.toStdString() << " siteID: " << this->_siteId << " couunterID: "
              << this->_counterId << std::endl;
    std::cout << "pos: [";
    for (auto x : this->pos) {
        std::cout << x << ", ";
    }
    std::cout << "]\n";
}

//TODO: forse sarà cancellato
void Symbol::writeStream(QTextStream &out) {
    out << this->_siteId;
    out << this->_counterId;
    //out<<this->pos;
    out << this->letter;
    out << this->font.font;
    //out<<this->font.foreground;
}


