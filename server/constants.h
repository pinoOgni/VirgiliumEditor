//
// Created by alex on 17/12/19.
//

#ifndef VIRGILIUMSERVER_CONSTANTS_H
#define VIRGILIUMSERVER_CONSTANTS_H

#include <QObject>

#define _int qint64
#define ConnectionWaitingTime 5000 // in millisecondi
#define Q_DATA_STREAM_VERSION QDataStream::Qt_5_1
#define LISTENING_PORT 9999
/*Lista di costanti che discrimina i segnali e quello che deve fare l'applicazione:
 * */
#define SITE_ID_ASSIGNMENT 1
#define SYMBOL_INSERT_OR_ERASE 2


#endif //VIRGILIUMSERVER_CONSTANTS_H
