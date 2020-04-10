//
// Created by alex on 12/12/19.
//

#include "symbol.h"
#include <iostream>
#include <algorithm>
#include <QDataStream>
symbol::symbol(QString letter, _int _siteId,_int _counterId,QVector<_int> pos):letter(letter),_siteId(_siteId),_counterId(_counterId), pos(pos) {
    //std::cout<<"Sono il symbol di SiteId: "<<this->_siteId<<" e counterId "<<this->_counterId<<\
    " il mio valore e' '"<<this->letter<<"' e il mio vettore di pos e':\n[ ";
    // std::for_each(this->pos.begin(),this->pos.end(),[](_int n){
    //   std::cout<<n<<", ";
    // });
    // std::cout<<"]\n";
}

QString symbol::getLetter() const {
    return this->letter;
}

_int symbol::getSiteId() const{
    return this->_siteId;
}

_int symbol::getCounterId() const {
    return this->_counterId;
}

QVector<_int> symbol::getPosition() const{
    return this->pos;
}

//override dellìoperatore == per un oggetto di tipo symbol sennò il compilatore ti dice "che succede coglione?"
bool symbol::operator==(const symbol& b) {
    return (this->_siteId==b._siteId && this->_counterId  == b._counterId && this->letter == b.letter && this->pos == b.pos );
}

//stessa cosa di prima solo per l'operatore "<"
//serve perchè quando arriva una insert di un carattere da un altro client e devo mettere il carattere nella posizione giusta del testo di questo (this) client
bool symbol::operator<(const symbol &b) {
    _int i;
    bool flag=false;
    //devo ritornare se this è  (strettamente) minore di b
    _int _max = ( this->pos.size() < b.pos.size() ) ? this->pos.size() : b.pos.size();
    //ciclo sul vettore più corto

    for(i=0;i<_max;i++){
        if(  this->pos[i] <  b.pos[i] )
            flag=true;
    }
    if(i==_max&&b.pos.size()>this->pos.size())
        flag=true;
    return flag;
}

//TODO: forse sarà cancellato
void symbol::writeStream(QDataStream &out) {
    out<<this->_siteId;
    out<<this->_counterId;
    out<<this->pos;
    out<<this->letter;
}
//stessa cosa di message: butto tutte cose su stream
QDataStream &operator<<(QDataStream &stream, const symbol &myclass) {
    stream << myclass._siteId;
    stream << myclass._counterId;
    stream << myclass.pos;
    stream << myclass.letter;
    return stream;
}
//stessa cosa di message: leggo tutte cose da stream
QDataStream &operator>>(QDataStream &stream, symbol &myclass) {
    stream >> myclass._siteId;
    stream >> myclass._counterId;
    stream >> myclass.pos;
    stream >> myclass.letter;
    return stream;
}
//TODO: forse sarà cancellato
symbol::symbol() {}

//TODO: da cancellare? per debug
void symbol::printSymbol() {
    std::string out="valore :";
    //QChar::to
    std::cout<<"valore: "<<this->letter.toStdString()<<" siteID: "<<this->_siteId<<" couunterID: "<<this->_counterId<<std::endl;
    std::cout<<"pos: [";
    for(auto x : this->pos){
        std::cout<<x<<", ";
    }
    std::cout<<"]\n";
}


