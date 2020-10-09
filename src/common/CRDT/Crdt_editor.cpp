//
// Created by alex on 07/12/19.
//

#include "Crdt_editor.h"
#include <utility>
#include <common/messages/StorageMessage.h>

Crdt_editor::Crdt_editor(QWidget *parent, ClientSocket *receivedSocket, QString fileName) :
        fileName(std::move(fileName)) {
    this->socket = receivedSocket;
    this->_counter = 0;

    connect(this->socket, &ClientSocket::storageMessageReceivedLoad, this, &Crdt_editor::loadResponse);
    connect(this->socket, &ClientSocket::crdtMessageReceived, this, &Crdt_editor::clientProcess);
    connect(this->socket, &ClientSocket::activeUserMessageReceived, this, &Crdt_editor::activeUserChanged);

    this->_siteId = this->socket->getClientID();
    qDebug() << "Il mio siteID è: " << this->_siteId;
}

Crdt_editor::Crdt_editor() = default;

Crdt_editor::~Crdt_editor() = default;

void Crdt_editor::clientProcess(_int code, const CrdtMessage &m) {
    for (Symbol symbol : m.getSymbols()) {
        _int i;

        if (m.getAction() == "INSERT") {
            auto newPos = symbol.getPosition();
            /*for (_int i = 0; i < this->_symbols.size(); i++) {
                if (this->_symbols[i].getPosition() == newPos)
                    //throw wrongpositionException();
                    ;//throw wrongpositionException();
            }*/

            for (i = 0; i < this->_symbols.size(); i++)
                if (symbol <= this->_symbols[i]) break; // position found
            auto it = this->_symbols.begin() + i;
            this->_symbols.insert(it, symbol);

            if (m.getMode())
                    emit insert_into_window(i, symbol.getLetter(), symbol.getFont(), symbol.getSiteId());
        } else if (m.getAction() == "ERASE") {
            for (i = 0; i < this->_symbols.size(); i++)
                if (symbol == this->_symbols[i]) break;

            if (this->_symbols.begin() + i == this->_symbols.end())
                return;
            auto it = this->_symbols.begin() + i;
            this->_symbols.erase(it);

            if (m.getMode())
                    emit remove_into_window(i, symbol.getSiteId());
        } else if (m.getAction() == "CURSOR_CHANGED") {
            emit change_cursor_position(symbol.getPosition().at(1), symbol.getSiteId());
        }
    }
}

QVector<Symbol> Crdt_editor::serverProcess(QVector<Symbol> symbols, const CrdtMessage &crdtMessage) {
    this->_symbols = std::move(symbols);
    this->clientProcess(0, crdtMessage);
    return this->_symbols;
}

//gli interi prev e next sono quelli che andranno a contenere quelli che sono gli elementi passati alla funzione
//i due vettori vengono controllati parallelemante
//_max gli si assegna il la dimensione maggiore tra i due vettori
QVector<_int> Crdt_editor::getPosition(QVector<_int> prec, QVector<_int> succ) {
    QVector<_int> nuovaPos;
    _int i, prev, next, _max = (prec.size() > succ.size()) ? prec.size() : succ.size();
    for (i = 0; i < _max; i++) {
        prev = (i >= prec.size()) ? 0 : prec[i];
        next = (i >= succ.size()) ? 9 : succ[i];

        if (prev == next) {
            nuovaPos.push_back(prev);
        } else {
            if (next - prev >= 2) {
                nuovaPos.push_back(prev + 1);
                break;
            } else {
                // aggiungiamo il caso in cui prev > next
                if (prev > next) {

                    // se prev != 9 allora possiamo incrementarlo
                    if (prev != 9) {
                        nuovaPos.push_back(prev + 1);
                        break;
                    } else if (next != 0) { // se prev = 9 allora proviamo a decrementare next
                        nuovaPos.push_back(next - 1);
                        break;
                    }
                }
                nuovaPos.push_back(prev);
            }
        }
    }
    if (i == _max) {
        nuovaPos.push_back(1);
    }
    return nuovaPos;
}

/* This method is used to send to other clients the new position of my cursor. */
void Crdt_editor::changeCursor(_int position) {
    qDebug() << "CURSOR";
    QVector<_int> pos = {position - 1, position};
    Symbol::CharFormat font = Symbol::CharFormat();
    Symbol s("", this->_siteId, this->_counter, pos, font);
    QVector<Symbol> symbols = {s};
    CrdtMessage m(this->_siteId, symbols, false, "CURSOR_CHANGED", this->fileName);
    if (sendCursor)
        this->socket->send(CURSOR_CHANGED, m);

    sendCursor = true;
}

/* This method is used to say to other clients that a char is deleted. */
void Crdt_editor::localErase(const QVector<_int> &indexes) {
    sendCursor = false;
    QVector<Symbol> symbols;
    for (_int index : indexes) {
        auto s = this->_symbols[index];
        auto it = this->_symbols.begin() + index;
        this->_symbols.erase(it);
        symbols.push_back(s);
    }
    CrdtMessage m(this->_siteId, symbols, false, "ERASE", this->fileName);
    this->socket->send(SYMBOL_INSERT_OR_ERASE, m);
}

/* This method is used to say to other clients that a char is inserted. */
void Crdt_editor::localInsert(QVector<_int> indexes, QVector<QString> values, QVector<Symbol::CharFormat> fonts) {
    qDebug() << "INSERT";
    sendCursor = false;
    QVector<_int> prev;
    QVector<_int> newPos;
    QVector<_int> succ;

    QVector<Symbol> symbolsToSend;
    for (int i = 0; i < indexes.size(); i++) {
        if (this->_symbols.empty() && indexes[i] == 0) {
            /* In this case there are no symbols in the document */
            prev.push_back(0);
            succ.push_back(2);
            // newPos.push_back(1);// lo zero non va mai messo come posizione
        } else {
            if (indexes[i] == 0) {
                /* The symbol must be inserted in head */
                prev.push_back(0);
                succ = this->_symbols[indexes[i]].getPosition();
            }
            if (indexes[i] == this->_symbols.size()) {
                /* The symbol must be inserted in the end */
                prev = this->_symbols[indexes[i] - 1].getPosition();
                succ.push_back(prev[0] + 2);
                // faccio prev[0]+2 così la mia funzione getPosition dovrebbe ficcare
                // il newSymbol symbol tra i due senza creare un vettorone ma creando un
                // vettore di un elemento di valore prev[0]+1 (spero)
            }
            if (indexes[i] != 0 && indexes[i] != this->_symbols.size()) {
                /* The symbol must be inserted in the middle */
                prev = this->_symbols[indexes[i] - 1].getPosition();
                succ = this->_symbols[indexes[i]].getPosition();
            }
        }

        newPos = this->getPosition(prev, succ);

        Symbol newSymbol(std::move(values[i]), this->_siteId, this->_counter++, newPos, fonts[i]);
        auto it = this->_symbols.begin() + indexes[i];
        this->_symbols.insert(it, newSymbol);

        symbolsToSend.push_back(newSymbol);
    }

    CrdtMessage m(this->_siteId, symbolsToSend, false, "INSERT", this->fileName);
    this->socket->send(SYMBOL_INSERT_OR_ERASE, m);
}

/*void Crdt_editor::set_site_id(qint64 siteId) {
    this->_siteId = siteId;
    qDebug() << "Il mio site ID e' : " << this->_siteId << "\n";
}*/

void Crdt_editor::loadRequest(const QString &name, User user) {
    user.setSiteId(this->_siteId);
    user.setLastCursorPos(0);

    QList<User> users = {user};
    QVector<Symbol> symbols;
    StorageMessage storageMessage(this->_siteId, symbols, name, users);
    this->socket->send(LOAD_REQUEST, storageMessage);
}

void Crdt_editor::loadResponse(_int code, StorageMessage storageMessage) {
    for (const Symbol &symbol : storageMessage.getSymbols())
        this->_symbols.push_back(symbol);

    QList<User> users;
    for (User u : storageMessage.getActiveUsers()) {
        u.setAssignedColor(QColor(QRandomGenerator::global()->bounded(64, 192),
                                  QRandomGenerator::global()->bounded(64, 192),
                                  QRandomGenerator::global()->bounded(64, 192)));
        users.push_back(u);
    }

    emit load_response(code, storageMessage.getSymbols(), users);
}

void Crdt_editor::deleteFromActive(const User &user, const QString &name) {
    UserMessage userMessage(this->_siteId, user, name);
    this->socket->send(DELETE_ACTIVE, userMessage);
}

void Crdt_editor::activeUserChanged(_int code, ActiveUserMessage activeUserMessage) {
    QList<User> users;
    for (User u : activeUserMessage.getActiveUsers()) {
        u.setAssignedColor(QColor(QRandomGenerator::global()->bounded(64, 192),
                                  QRandomGenerator::global()->bounded(64, 192),
                                  QRandomGenerator::global()->bounded(64, 192)));
        users.push_back(u);
    }
    emit change_active_users(users);
}
