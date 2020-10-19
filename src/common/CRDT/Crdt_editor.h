#ifndef VIRGILIUMCLIENT_VIRGILIUM_CLIENT_H
#define VIRGILIUMCLIENT_VIRGILIUM_CLIENT_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include "common/constants.h"
#include "common/messages/CrdtMessage.h"
#include <vector>
#include <string>
#include <QtWidgets>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include "common/ClientSocket.h"

class symbol;

class Crdt_editor : public QObject {
Q_OBJECT

    ClientSocket *socket;
    QString fileName;
    _int _siteId;
    QVector<Symbol> _symbols;
    _int _counter;
    bool sendCursor = true;
    void changeFirstSymbol(_int index);

public:
    Crdt_editor(QWidget *parent, ClientSocket *receivedSocket, QString fileName);

    Crdt_editor();

    ~Crdt_editor();

    void localInsert(_int index, QString value, const Symbol::CharFormat &font);

    void localErase(_int index);

    void changeCursor(_int position);

    void changeBlockFormat(const Symbol::CharFormat &font);

    /*void changeBlockFormat(const Symbol::CharFormat &font, _int startPos, _int finalPos);*/

    void loadRequest(const QString &fileName, User user);

    void deleteFromActive(const User &user, const QString &fileName);

    QVector<Symbol> serverProcess(QVector<Symbol> symbols, const CrdtMessage &crdtMessage);

    QVector<_int> getPosition(QVector<_int> prec, QVector<_int> succ);

    void localUpdate(_int pos, const Symbol::CharFormat &charData);

public slots:

    void loadResponse(_int code, StorageMessage storageMessage);

    void clientProcess(_int code, const CrdtMessage &m);

    void activeUserChanged(_int code, ActiveUserMessage activeUserMessage);

signals:

    void insert_into_window(_int pos, QString character, Symbol::CharFormat font, _int siteId);

    void remove_into_window(_int pos, _int siteId);

    void change_cursor_position(_int position, _int siteId);

    void load_response(_int code, QVector<Symbol> symbols, QList<User> users);

    void change_active_users(QList<User> users);

    /*void change_block_format(QString font, _int startPos, _int finalPos);*/
    void change_block_format(QString font);

    void change_char_format(_int i, Symbol::CharFormat charData);
};

#endif //VIRGILIUMCLIENT_VIRGILIUM_CLIENT_H
