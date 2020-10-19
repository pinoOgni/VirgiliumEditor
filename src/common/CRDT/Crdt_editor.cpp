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
    //spdlog::debug("Il mio siteID è: {} ", this->_siteId);
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
                if (symbol <= this->_symbols[i]) break;
            auto it = this->_symbols.begin() + i;
            this->_symbols.insert(it, symbol);

            if (m.getMode())
                    emit insert_into_window(i, symbol.getLetter(), symbol.getFont(), m.getSender());
        } else if (m.getAction() == "ERASE") {
            for (i = 0; i < this->_symbols.size(); i++)
                if (symbol == this->_symbols[i]) break;

            this->changeFirstSymbol(i);

            if (this->_symbols.begin() + i == this->_symbols.end())
                return;
            auto it = this->_symbols.begin() + i;
            this->_symbols.erase(it);

            if (m.getMode())
                    emit remove_into_window(i, m.getSender());
        } else if (m.getAction() == "CURSOR_CHANGED") {
            emit change_cursor_position(symbol.getPosition().at(1), m.getSender());
        } else if (m.getAction() == "CHANGE_BLOCK_FORMAT") {
            if (!this->_symbols.isEmpty()) {
                Symbol firstSymbol = this->_symbols.at(0);
                Symbol::CharFormat format = m.getSymbol().getFont();
                Symbol newSymbol(firstSymbol.getLetter(), firstSymbol.getSiteId(), firstSymbol.getCounterId(),
                                 firstSymbol.getPosition(), format);
                this->_symbols.removeFirst();
                this->_symbols.prepend(newSymbol);
            }

            if (m.getMode())
                    emit change_block_format(m.getSymbol().getFont().font);
        } else if (m.getAction() == "UPDATE") {
            Symbol s = m.getSymbol();

            _int i = -1;
            for (i = 0; i < this->_symbols.size(); i++) {
                if (this->_symbols[i] == s)
                    break;
            }

            if (i == this->_symbols.size()) return;

            Symbol oldSymbol = this->_symbols.at(i);

            QRegExp tagExp(",");
            QStringList newList = s.getFont().font.split(tagExp);
            QStringList oldList = oldSymbol.getFont().font.split(tagExp);

            QString newFont;
            bool changed = true;
            for (int i = 0; i < 10; i++) {
                if (newList.at(i) == "-1") {
                    newFont += oldList.at(i) + ",";
                } else {
                    changed = false;
                    newFont += newList.at(i) + ",";
                }

                if (i == 9) newFont.remove(newFont.lastIndexOf(","), 1);
            }

            Symbol::CharFormat charData;
            charData.font = newFont;
            if (changed)
                charData.foreground = s.getFont().foreground;
            else
                charData.foreground = oldSymbol.getFont().foreground;

            Symbol newSymbol(oldSymbol.getLetter(), oldSymbol.getSiteId(), oldSymbol.getCounterId(),
                             oldSymbol.getPosition(), charData);
            this->_symbols.remove(i);
            this->_symbols.insert(i, newSymbol);

            if (m.getMode())
                    emit change_char_format(i, charData);
        }
    }
}

QVector<Symbol> Crdt_editor::serverProcess(QVector<Symbol> symbols, const CrdtMessage &crdtMessage) {
    this->_symbols = std::move(symbols);
    this->clientProcess(0, crdtMessage);
    return this->_symbols;
}

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
                if (prev > next) {
                    if (prev != 9) {
                        newPos.push_back(prev + 1);
                        break;
                    } else if (next != 0) {
                        newPos.push_back(next - 1);
                        break;
                    }
                }
                newPos.push_back(prev);
            }
        }
    }
    if (i == _max)
        newPos.push_back(1);
    return newPos;
}

void Crdt_editor::changeCursor(_int position) {
    QVector<_int> pos = {position - 1, position};
    Symbol::CharFormat font = Symbol::CharFormat();
    Symbol s("", this->_siteId, this->_counter, pos, font);
    CrdtMessage m(this->_siteId, s, false, "CURSOR_CHANGED", this->fileName);
    if (sendCursor)
        this->socket->send(CURSOR_CHANGED, m);

    sendCursor = true;
}

void Crdt_editor::localErase(_int index) {
    sendCursor = false;

    this->changeFirstSymbol(index);
    auto s = this->_symbols[index];
    auto it = this->_symbols.begin() + index;
    this->_symbols.erase(it);
    CrdtMessage m(this->_siteId, s, false, "ERASE", this->fileName);
    this->socket->send(SYMBOL_INSERT_OR_ERASE, m);
}

void Crdt_editor::localInsert(_int index, QString value, const Symbol::CharFormat &font) {
    sendCursor = false;
    QVector<_int> prev;
    QVector<_int> newPos;
    QVector<_int> succ;

    if (this->_symbols.empty() && index == 0) {
        prev.push_back(0);
        succ.push_back(2);
    } else {
        if (index == 0) {
            prev.push_back(0);
            succ = this->_symbols[index].getPosition();
        }
        if (index == this->_symbols.size()) {
            prev = this->_symbols[index - 1].getPosition();
            succ.push_back(prev[0] + 2);
        }
        if (index != 0 && index != this->_symbols.size()) {
            prev = this->_symbols[index - 1].getPosition();
            succ = this->_symbols[index].getPosition();
        }
    }

    newPos = this->getPosition(prev, succ);

    Symbol newSymbol(std::move(value), this->_siteId, this->_counter++, newPos, font);
    auto it = this->_symbols.begin() + index;
    this->_symbols.insert(it, newSymbol);
    CrdtMessage m(this->_siteId, newSymbol, false, "INSERT", this->fileName);
    this->socket->send(SYMBOL_INSERT_OR_ERASE, m);
}

void Crdt_editor::changeBlockFormat(const Symbol::CharFormat &font) {
    Symbol::CharFormat charData;
    Symbol first = this->_symbols[0];
    charData.foreground = first.getFont().foreground;
    charData.font = font.font;
    Symbol newSymbol(first.getLetter(), first.getSiteId(), first.getCounterId(), first.getPosition(), charData);
    this->_symbols.erase(this->_symbols.begin());
    this->_symbols.prepend(newSymbol);

    QVector<_int> pos;
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

    QList<User> users;
    for (User u : storageMessage.getActiveUsers()) {
        u.setAssignedColor(QColor(QRandomGenerator::global()->bounded(64, 192),
                                  QRandomGenerator::global()->bounded(64, 192),
                                  QRandomGenerator::global()->bounded(64, 192)));
        users.push_back(u);
    }
    this->activeUsers = users;

    emit load_response(code, storageMessage.getSymbols(), users);
}

void Crdt_editor::deleteFromActive(const User &user, const QString &name) {
    UserMessage userMessage(this->_siteId, user, name);
    this->socket->send(DELETE_ACTIVE, userMessage);
}

void Crdt_editor::activeUserChanged(_int code, ActiveUserMessage activeUserMessage) {
    if (activeUserMessage.getActiveUsers().size() > this->activeUsers.size()) {
        for (User user : activeUserMessage.getActiveUsers()) {
            if (!this->activeUsers.contains(user)) {
                user.setAssignedColor(QColor(QRandomGenerator::global()->bounded(64, 192),
                                             QRandomGenerator::global()->bounded(64, 192),
                                             QRandomGenerator::global()->bounded(64, 192)));
                user.setLastCursorPos(0);
                this->activeUsers.push_back(user);
            }
        }
    } else if (activeUserMessage.getActiveUsers().size() < this->activeUsers.size()) {
        for (const User &user : this->activeUsers) {
            if (!activeUserMessage.getActiveUsers().contains(user))
                this->activeUsers.removeOne(user);
        }
    }

    emit change_active_users(this->activeUsers);
}

void Crdt_editor::localUpdate(_int pos, const Symbol::CharFormat &charData) {
    QRegExp tagExp(",");
    QStringList newList = charData.font.split(tagExp);

    Symbol s = this->_symbols.at(pos);
    QStringList oldList = s.getFont().font.split(tagExp);

    QString newFont;
    for (int i = 0; i < 10; i++) {
        if (newList.at(i) == oldList.at(i))
            newFont += "-1,";
        else
            newFont += newList.at(i) + ",";

        if (i == 9) newFont.remove(newFont.lastIndexOf(","), 1);
    }

    Symbol newSymbol(s.getLetter(), s.getSiteId(), s.getCounterId(), s.getPosition(), charData);
    this->_symbols.remove(pos);
    this->_symbols.insert(pos, newSymbol);

    Symbol::CharFormat charDataToSend;
    charDataToSend.font = newFont;
    charDataToSend.foreground = charData.foreground;
    Symbol symbolToSend(s.getLetter(), s.getSiteId(), s.getCounterId(), s.getPosition(), charDataToSend);
    CrdtMessage m(this->_siteId, symbolToSend, false, "UPDATE", this->fileName);
    this->socket->send(SYMBOL_INSERT_OR_ERASE, m);
}

void Crdt_editor::changeFirstSymbol(_int index) {
    if (index == 0) {
        QString format = this->_symbols[0].getFont().font;
        auto second = this->_symbols[1];
        Symbol::CharFormat charData;
        charData.foreground = second.getFont().foreground;
        charData.font = format;
        Symbol newSymbol(second.getLetter(), second.getSiteId(), second.getCounterId(), second.getPosition(),
                         charData);
        this->_symbols.erase(this->_symbols.begin() + 1);
        this->_symbols.insert(1, newSymbol);
    }
}