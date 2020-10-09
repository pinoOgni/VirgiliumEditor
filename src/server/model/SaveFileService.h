//
// Created by alex on 09/10/20.
//

#ifndef VIRGILIUM_SAVEFILESERVICE_H
#define VIRGILIUM_SAVEFILESERVICE_H


#include <QtCore/QRunnable>
#include "Model.h"

class SaveFileService : public QRunnable {
    Model &model;
    CrdtMessage crdtMessage;
public:
    SaveFileService(Model &model, CrdtMessage crdtMessage);

    void run() override;

};


#endif //VIRGILIUM_SAVEFILESERVICE_H
