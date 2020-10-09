//
// Created by alex on 09/10/20.
//

#include "SaveFileService.h"

SaveFileService::SaveFileService(Model &model, CrdtMessage crdtMessage) : model(model), crdtMessage(crdtMessage) {
}

void SaveFileService::run() {
    auto symbols = this->model.getSymbolsForDocument(crdtMessage.getFileName());
    auto toBeSaved = this->model.performServerProcess(symbols, crdtMessage);

    this->model.updateSymbolsForDocument(crdtMessage.getFileName(), toBeSaved);

}
