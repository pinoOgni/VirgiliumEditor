//
// Created by alex on 07/12/19.
//

#ifndef VIRGILIUMCLIENT_CONSTANTS_H
#define VIRGILIUMCLIENT_CONSTANTS_H

/*
quando si comunica con i socket, bisogna sapere la quantità di dati trasmessa.
qint64 migliore dell'int nativo di C++ perché non ha problemi con le vecchie architetture.
qint64 = 64 bit fissi su tutte le piattaforme
da qui, in futuro, se serve si può modificare subito
*/
#define _int qint64

/*
ConnectionWaitingTime la usa solo il client, viene usata solo quando si connete al server.
Aspetta 5 secondi per dire se ci sono problemi (magari non trova il server)
*/
#define ConnectionWaitingTime 5000 // in millisecondi

/*
Q_DATA_STREAM_VERSION serve per gli stream, ossia quando si legge dal socket.
Bisogna sempre mettere la versione, dato che gli oggetti di tipo qdataastream riescono a mettersi d'accordo tra di loro
*/
#define Q_DATA_STREAM_VERSION QDataStream::Qt_5_1

/*Lista di costanti che discrimina i segnali e quello che deve fare l'applicazione:
 * */

/*
Sia il client che il server la prima cosa che fanno è scrivere sul socket un _int (definito prima). Serve a discriminare quello che fa il client

SITE_ID_ASSIGNMENT: serve a discriminare quando ancora il client non ha un site_id (per capirlo serve sapere cosa fa il serve quando gli arriva una nuova connessione)

SYMBOL_INSERT_OR_ERASE: il client ha ricevuto un simbolo da trasferire. Il client allora sa che deve leggere un oggetto di tipo message (come il laboratorio 3)

*/
#define SITE_ID_ASSIGNMENT 1
#define SYMBOL_INSERT_OR_ERASE 2



#endif //VIRGILIUMCLIENT_CONSTANTS_H
