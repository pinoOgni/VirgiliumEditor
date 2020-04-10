//
// Created by alex on 07/12/19.
//
/*   BENVENUTO nel codice sorgente del client di Virgilium!
 *  qua una breve descrizione dei membri privati di questa classe che modella una istanza del client
 *
 *  this->server è una classa che incaspula un socket verso il server (più un buffer per leggere i dati
 *  in ingresso).
*   Quando viene istanziato un oggetto di classe virgilium_client questo socket contatta il server il
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
#include "constants.h"
#include "server_virgilium.h"
#include <vector>
#include <string>
#include <QtWidgets>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>

class symbol;

class virgilium_client : public QDialog {
    Q_OBJECT

    server_virgilium *server; // ------------> QTcpServer *server;


    _int _siteId;
    std::vector<symbol> _symbols;
    _int _counter;

    QComboBox *hostCombo = nullptr;
    QLineEdit *portLineEdit = nullptr;
    QLabel *statusLabel = nullptr;
    QPushButton *getFortuneButton = nullptr;


    QVector<_int> getPosition(QVector<_int> prec,QVector<_int> succ);

public:
    explicit virgilium_client(QWidget *parent=0, QString ServerName=0, _int serverPort=0);
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
    ~virgilium_client();
    _int get_id() const;
    void localInsert(_int index, QString value);
    void process(const message& m);
    QString to_string();
    void localErase(_int index);
    void set_site_id(_int siteId);
    void mandaqualcosa();
public slots:
    void faiLocalInsert();
    signals:
    void site_id_assignment();


};


#endif //VIRGILIUMCLIENT_VIRGILIUM_CLIENT_H
