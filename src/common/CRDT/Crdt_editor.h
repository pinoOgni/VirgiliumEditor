//
// Created by alex on 07/12/19.
//
/*   BENVENUTO nel codice sorgente del client di Virgilium!
 *  qua una breve descrizione dei membri privati di questa classe che modella una istanza del client
 *
 *  this->server è una classa che incaspula un socket verso il server (più un buffer per leggere i dati
 *  in ingresso).
*   Quando viene istanziato un oggetto di classe Crdt_editor questo socket contatta il server il
 *   cui indirizzo è hard-coded (per ora) in questi sorgenti. Il server ascolta sulla interfaccia di
 *   loopback (per ora).
 *   this->_siteId è lo stesso siteId del lab3
 *   this->_symbols è lo stesso vettore di simboli del lab3
 *   this->counter è lo stesso counter del lab3
 *
 * */
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

    ClientSocket *socket{};
    QString fileName;
    _int _siteId;
    QVector<Symbol> _symbols;
    _int _counter;
    boolean sendCursor = true;

public:
    Crdt_editor(QWidget *parent, ClientSocket *receivedSocket, QString fileName);

    Crdt_editor();

    /* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
     * Questo è il costruttore del client, è un oggetto QT, prende in input una stringa con l'ip del
     * server (hard-coded(per ora)) e la porta (hard-coded(per ora)).
     * Per come ho fatto il server ora quest'ultimo appena vede che un client si vuole connettere
     * lo aggiunge alla propria lista e gli manda il suo siteID.
     * Quindi a costruttore chiamato noi abbiamo il nostro bel client con il suo site_id e con il socket connesso
     * al server
     * Quando verrà il momento bisognerà farcire tutto di eccezioni.
     *
     *
     * */
    ~Crdt_editor();

    void localInsert(QVector<_int> indexes, QVector<QString> values, QVector<Symbol::CharFormat> fonts);

    void localErase(const QVector<_int> &indexes);

    void changeCursor(_int position);

    /*void set_site_id(_int siteId);*/

    void loadRequest(const QString &fileName, User user);

    void deleteFromActive(const User &user, const QString &fileName);

    QVector<Symbol> serverProcess(QVector<Symbol> symbols, const CrdtMessage &crdtMessage);

    QVector<_int> getPosition(QVector<_int> prec, QVector<_int> succ);

public slots:

    void loadResponse(_int code, StorageMessage storageMessage);

    void clientProcess(_int code, const CrdtMessage &m);

    void activeUserChanged(_int code, ActiveUserMessage activeUserMessage);

signals:

    /*void site_id_assignment();*/

    void insert_into_window(_int pos, QString character, Symbol::CharFormat font, _int siteId);

    void remove_into_window(_int pos, _int siteId);

    void change_cursor_position(_int position, _int siteId);

    void load_response(_int code, QVector<Symbol> symbols, QList<User> users);

    void change_active_users(QList<User> users);

};

#endif //VIRGILIUMCLIENT_VIRGILIUM_CLIENT_H
