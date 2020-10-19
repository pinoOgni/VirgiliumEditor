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
    QList<User> activeUsers;

    /*
     * This is a private method to modify the first symbol when the alignment/indentation is modified
     */
    void changeFirstSymbol(_int index);

public:
    Crdt_editor(QWidget *parent, ClientSocket *receivedSocket, QString fileName);

    Crdt_editor();

    ~Crdt_editor();

    /*
     * This method is used to say to other clients that a char is inserted. Here, the
     * position of the new symbol is calculated by using the getPosition method.
     */
    void localInsert(_int index, QString value, const Symbol::CharFormat &font);

    /*
     * This method is used to say to the other clients that a char is deleted. Also the
     * local copy of symbols is updated.
     */
    void localErase(_int index);

    /*
     * This method is used to send to other clients the new position of my cursor.
     */
    void changeCursor(_int position);

    /*
     * This method is used to sent to the other clients the new BlockFormat, in particular
     * alignment and indentation information are sent.
     */
    void changeBlockFormat(const Symbol::CharFormat &font);

    /*
     * This method is used to request the vector of symbols and the list of active users
     * when the editor is opened.
     */
    void loadRequest(const QString &fileName, User user);

    /*
     * This method is used to delete the current user from the list of active users when
     * the editor is closed.
     */
    void deleteFromActive(const User &user, const QString &fileName);

    /*
     * This method is used by the server in order to get the new vector of symbols, after
     * that the vector is saved inside the file system.
     */
    QVector<Symbol> serverProcess(QVector<Symbol> symbols, const CrdtMessage &crdtMessage);

    /*
     * This method is used to calculate the position of a new symbol.
     */
    QVector<_int> getPosition(QVector<_int> prec, QVector<_int> succ);

    /*
     * This method is used to update the font of the char in position pos, after that
     * a message is sent to the server in order to inform the other clients.
     */
    void localUpdate(_int pos, const Symbol::CharFormat &charData);

public slots:

    /*
     * This slot is used to update the vector of symbols and the list of active users.
     */
    void loadResponse(_int code, StorageMessage storageMessage);

    /*
     * This method is used to modify the _symbols vector when a new character is
     * inserted/removed or when the font of the symbol is changed.
     */
    void clientProcess(_int code, const CrdtMessage &m);

    /*
     * This slot is invoked every time the list of active users is changed.
     */
    void activeUserChanged(_int code, ActiveUserMessage activeUserMessage);

signals:

    void insert_into_window(_int pos, QString character, Symbol::CharFormat font, _int siteId);

    void remove_into_window(_int pos, _int siteId);

    void change_cursor_position(_int position, _int siteId);

    void load_response(_int code, QVector<Symbol> symbols, QList<User> users);

    void change_active_users(QList<User> users);

    void change_block_format(QString font);

    void change_char_format(_int i, Symbol::CharFormat charData);
};

#endif //VIRGILIUMCLIENT_VIRGILIUM_CLIENT_H
