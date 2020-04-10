//
// Created by alex on 12/12/19.
//

#include <iostream>
#include "message.h"

message::message(symbol s, _int from, QString action): s(s),from(from),action(action){};

//ti ritorna il site_id del client che ha fatto la richiesta
_int  message::getFrom() const{
    return this->from;
}
//ti ritorna l'azione che fa il client che ha fatto la richiesta (tipo che ha scritto un carattere)
QString message::getAction() const {
    return this->action;
}

symbol message::getSymbol() const {
    return this->s;
}

//solo per debug
void message::printMessage() {
    std::cout<<"Messaggio inviato da: "<<this->from<<"action=\""<<this->action.toStdString()<<"\""<<std::endl;
    std::cout<<"symbol:\n";
    this->s.printSymbol();

}

//scrive sullo stream un simbolo, un from e un action (this client è il mittente)
QDataStream &operator<<(QDataStream &stream, const message &myclass) {
    stream << myclass.s;
    stream << myclass.from;
    stream << myclass.action;
    //stream << myclass.letter;
    return stream;
}
//legge dallo stream un simbolo, un from e un action (this client è il ricevente)
QDataStream &operator>>(QDataStream &stream, message &myclass) {
    stream >> myclass.s; 
    stream >> myclass.from;
    stream >> myclass.action;
    return stream;
}

//messaggio vuoto, usato per "leggere" qualcosa dal socket (serve a leggere dei byte dal socket, come struttura per popolare il message per quando arriva il vero messaggio dal socket)
//chiedere ad Ale quando sarà meno ubriaco
message::message() {

}

