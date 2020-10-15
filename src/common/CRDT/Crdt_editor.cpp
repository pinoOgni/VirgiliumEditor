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
    spdlog::debug("Il mio siteID è: {} ", this->_siteId);
}

Crdt_editor::Crdt_editor() = default;

Crdt_editor::~Crdt_editor() = default;

void Crdt_editor::clientProcess(_int code, const CrdtMessage &m) {
    if (code == 0 || this->fileName == m.getFileName()) {
        auto symbol = m.getSymbol();
        _int i;

        if (m.getAction() == "INSERT") {
            auto newPos = m.getSymbol().getPosition();

            for (i = 0; i < this->_symbols.size(); i++)
                if (symbol <= this->_symbols[i]) break; // position found
            auto it = this->_symbols.begin() + i;
            this->_symbols.insert(it, symbol);

            if (m.getMode())
                    emit insert_into_window(i, symbol.getLetter(), symbol.getFont(), m.getSender());
        } else if (m.getAction() == "ERASE") {
            for (i = 0; i < this->_symbols.size(); i++)
                if (symbol == this->_symbols[i]) break;

            if (this->_symbols.begin() + i == this->_symbols.end())
                return;
            auto it = this->_symbols.begin() + i;
            this->_symbols.erase(it);

            if (m.getMode())
                    emit remove_into_window(i, m.getSender());
        } else if (m.getAction() == "CURSOR_CHANGED") {
            emit change_cursor_position(symbol.getPosition().at(1), m.getSender());
        } else if (m.getAction() == "CHANGE_BLOCK_FORMAT") {

            int i = m.getSymbol().getPosition().at(0);
            for (i; i <= m.getSymbol().getPosition().at(1); i++) {
                spdlog::debug("i: {}", i);
                Symbol s = this->_symbols.at(i);
                Symbol::CharFormat format = m.getSymbol().getFont();
                Symbol newSymbol(s.getLetter(), s.getSiteId(), s.getCounterId(), s.getPosition(), format);
                this->_symbols.remove(i);
                this->_symbols.insert(i, newSymbol);
                if (m.getMode())
                        emit change_block_format(newSymbol.getFont().font, i, i + 1);
            }
            if (m.getMode() && i < this->_symbols.size())
                    emit change_block_format(this->_symbols.at(i).getFont().font, i, i + 1);

            /*if (m.getMode())
                    emit change_block_format(m.getSymbol().getFont().font, m.getSymbol().getPosition().at(0),
                                             m.getSymbol().getPosition().at(1));*/
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
    QVector<_int> newPos;
    _int i, prev, next, _max = (prec.size() > succ.size()) ? prec.size() : succ.size();
    for (i = 0; i < _max; i++) {
        prev = (i >= prec.size()) ? 0 : prec[i];
        next = (i >= succ.size()) ? 9 : succ[i];

        if (prev == next) {
            newPos.push_back(prev);
        } else {
            if (next - prev >= 2) {
                newPos.push_back(prev + 1);
                break;
            } else {
                // aggiungiamo il caso in cui prev > next
                if (prev > next) {

                    // se prev != 9 allora possiamo incrementarlo
                    if (prev != 9) {
                        newPos.push_back(prev + 1);
                        break;
                    } else if (next != 0) { // se prev = 9 allora proviamo a decrementare next
                        newPos.push_back(next - 1);
                        break;
                    }
                }
                newPos.push_back(prev);
            }
        }
    }
    if (i == _max) {
        newPos.push_back(1);
    }
    return newPos;
}

/* This method is used to send to other clients the new position of my cursor. */
void Crdt_editor::changeCursor(_int position) {
    QVector<_int> pos = {position - 1, position};
    Symbol::CharFormat font = Symbol::CharFormat();
    Symbol s("", this->_siteId, this->_counter, pos, font);
    CrdtMessage m(this->_siteId, s, false, "CURSOR_CHANGED", this->fileName);
    if (sendCursor)
        this->socket->send(CURSOR_CHANGED, m);

    sendCursor = true;
}

/* This method is used to say to other clients that a char is deleted. */
void Crdt_editor::localErase(_int index) {
    sendCursor = false;
    auto s = this->_symbols[index];
    auto it = this->_symbols.begin() + index;
    this->_symbols.erase(it);
    CrdtMessage m(this->_siteId, s, false, "ERASE", this->fileName);
    this->socket->send(SYMBOL_INSERT_OR_ERASE, m);
}

/* This method is used to say to other clients that a char is inserted. */
void Crdt_editor::localInsert(_int index, QString value, Symbol::CharFormat font) {
    sendCursor = false;
    QVector<_int> prec; //= this->_symbols[index-1];
    QVector<_int> nuovaPos;
    QVector<_int> succ; //auto succ //= this-> _symbols[index];

    if (this->_symbols.empty() && index == 0) {
        //primo elemento inserito
        prec.push_back(0);
        succ.push_back(2);
        // nuovaPos.push_back(1);// lo zero non va mai messo come posizione
    } else {
        if (index == 0) {
            //inserisco in testa
            //std::vector<int> zeroes;
            prec.push_back(0);
            succ = this->_symbols[index].getPosition();
        }
        if (index == this->_symbols.size()) {
            //ultimo a destra
            prec = this->_symbols[index - 1].getPosition();
            succ.push_back(prec[0] + 2);
            // faccio prec[0]+2 così la mia funzione getPosition dovrebbe ficcare
            // il newSymbol symbol tra i due senza creare un vettorone ma creando un
            // vettore di un elemento di valore prec[0]+1 (spero)
        }
        if (index != 0 && index != this->_symbols.size()) {
            //caso medio
            //    std::cout<<"sto inserendo " << value<<std::endl;
            prec = this->_symbols[index - 1].getPosition();
            succ = this->_symbols[index].getPosition();
        }
    }

    nuovaPos = this->getPosition(prec, succ);

    Symbol newSymbol(std::move(value), this->_siteId, this->_counter++, nuovaPos, font);
    auto it = this->_symbols.begin() + index;
    this->_symbols.insert(it, newSymbol);
    CrdtMessage m(this->_siteId, newSymbol, false, "INSERT", this->fileName);
    this->socket->send(SYMBOL_INSERT_OR_ERASE, m);
}

void Crdt_editor::changeBlockFormat(const Symbol::CharFormat &font, _int startPos, _int finalPos) {
    QVector<_int> pos = {startPos, finalPos};
    Symbol s("", this->_siteId, this->_counter, pos, font);
    CrdtMessage m(this->_siteId, s, false, "CHANGE_BLOCK_FORMAT", this->fileName);
    this->socket->send(SYMBOL_INSERT_OR_ERASE, m);
}

void Crdt_editor::loadRequest(const QString &name, User user) {
    user.setSiteId(this->_siteId);
    user.setLastCursorPos(0);

    QList<User> users = {user};
    QVector<Symbol> symbols;
    StorageMessage storageMessage(this->_siteId, symbols, name, users);
    this->socket->send(LOAD_REQUEST, storageMessage);
}

void Crdt_editor::loadResponse(_int code, StorageMessage storageMessage) {
    this->_symbols = storageMessage.getSymbols();
    /*for (const Symbol &symbol : storageMessage.getSymbols())
        this->_symbols.push_back(symbol);*/

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
    emit change_active_users(activeUserMessage.getActiveUsers());
}
