//
// Created by alex on 09/10/20.
//

#include "SaveFileService.h"

#include <utility>

SaveFileService::SaveFileService(Model &model, CrdtMessage crdtMessage, QVector<Symbol> symbols) :
        model(model),
        crdtMessage(std::move(crdtMessage)),
        symbols(std::move(symbols)) {}

void SaveFileService::run() {
    this->model.updateSymbolsForDocument(crdtMessage.getFileName(), this->symbols);
}
