//
// Created by pinoOgni on 26/09/20.
//

#include "Model.h"

Model::Model() : IDSeed(1) {
    this->editor = new Crdt_editor();
}

void Model::insertLoggedUser(ClientSocket *socket, const User &user) {
    clientToUser.insert(std::pair<_int, ClientSocket *>(user.getSiteId(), socket));
}

void Model::removeLoggedUser(ClientSocket *socket) {
    auto it = clientToUser.begin();
    for (it; it != clientToUser.end(); it++)
        if (it->first == socket->getClientID()) break;

    clientToUser.erase(it);
}

ClientSocket *Model::getLoggedUser(User &user) {
    auto it = std::find_if(clientToUser.begin(), clientToUser.end(), [user](std::pair<_int, ClientSocket *> pair) {
        return pair.first == user.getSiteId();
    });
    if (it == clientToUser.end()) {
        return nullptr;
    } else {
        return it->second;
    }
}

void Model::insertSymbolsForDocument(const QString &fileName, const QVector<Symbol> &symbols) {
    symbolsForDocument.insert(std::pair<QString, QVector<Symbol>>(fileName, symbols));
}

void Model::removeSymbolsForDocument(const QString &fileName) {
    symbolsForDocument.erase(fileName);
}

QVector<Symbol> Model::getSymbolsForDocument(const QString &fileName) {
    auto it = symbolsForDocument.find(fileName);
    QVector<Symbol> symbols;
    if (it == symbolsForDocument.end())
        symbols = QVector<Symbol>();
    else
        symbols = symbolsForDocument[fileName];
    return symbols;
}

bool Model::loginUser(User user) {
    return Database::getInstance().loginDB(user);
}

bool Model::signinUser(User user) {
    return Database::getInstance().signinDB(user);
}

User Model::getInfoUser(User user) {
    return Database::getInstance().getInfoUser(user);
}

std::vector<FilesMessage> Model::getFilesOwner(User user) {
    return Database::getInstance().getFilesOwner(user);
}

std::vector<FilesMessage> Model::getUserFiles(User user) {
    return Database::getInstance().getUserFiles(user);
}

/*
void Model::getAllData(User user, User &userReturn, std::vector<FilesMessage> filesOwner,
                       std::vector<FilesMessage> filesCollabs) {
    userReturn = Database::getInstance().getInfoUser(user);
    filesOwner = Database::getInstance().getFilesOwner(user);
    filesCollabs = Database::getInstance().getUserFiles(user);
}
*/

bool Model::renameFile(FileManagementMessage fileManagementMessage) {
    return Database::getInstance().renameFileDB(fileManagementMessage);
}

bool Model::deleteFile(FileManagementMessage fileManagementMessage) {
    return Database::getInstance().deleteFileDB(fileManagementMessage);
}

bool Model::newFile(FileManagementMessage fileManagementMessage) {
    return Database::getInstance().newFileDB(fileManagementMessage);
}

bool Model::changePassword(ChangePasswordMessage changePasswordMessage) {
    return Database::getInstance().changePasswordDB(changePasswordMessage);
}

bool Model::addCollaborator(UserManagementMessage userManagementMessage) {
    //return Database::getInstance().addCollaboratorDB(userManagementMessage);
}

bool Model::removeCollaborator(UserManagementMessage userManagementMessage) {
    return Database::getInstance().removeCollaboratorDB(userManagementMessage);
}

bool Model::unsubscribe(UserManagementMessage userManagementMessage) {
    return Database::getInstance().unsubscribeDB(userManagementMessage);
}

void Model::closeConnectionDB() {
    Database::getInstance().closeConnectionDB();
}

QString Model::createUrlCollaborator(UserManagementMessage userManagementMessage) {
    return Database::getInstance().createUrlCollaboratorDB(userManagementMessage);
}

bool Model::requestToCollaborate(InvitationMessage invitationMessage) {
    return Database::getInstance().requestToCollaborateDB(invitationMessage);
}

QList<User> Model::addActiveUser(const User &user, const QString &fileName) {
    auto it = activeClientsForDocument.find(fileName); //TODO sostituire filename con id del file
    if (it != activeClientsForDocument.end()) { /* file already opened */
        activeClientsForDocument.at(fileName).push_back(user);
        return activeClientsForDocument.at(fileName);
    } else {
        QList<User> users = {user};
        activeClientsForDocument.insert(std::pair<QString, QList<User>>(fileName, users));
        return users;
    }
}

QList<User> Model::removeActiveUser(const User &user, const QString &fileName) {
    auto it = activeClientsForDocument.find(fileName);
    if (it == activeClientsForDocument.end())
        return QList<User>();

    activeClientsForDocument.at(fileName).removeOne(user);
    return activeClientsForDocument[fileName];
}

/*void Model::removeUserFromEditor(ClientSocket *socket) {
    auto toRemove = std::find_if(fileToClients.begin(), fileToClients.end(),
                                 [socket](auto &pair) {
                                     return socket == pair.second;
                                 }
    );
    fileToClients.erase(toRemove);
}

void Model::insertUserIntoEditor(ServerDocument *serverDocument, ClientSocket *socket) {
    this->fileToClients.insert(
            std::pair<ServerDocument *, ClientSocket *>(serverDocument, socket)
    );
}*/

void Model::insertMessage(const CrdtMessage &message) {
    this->messages.push_back(message);
}

QQueue<CrdtMessage> &Model::getMessages() {
    return this->messages;
}

std::map<QString, QList<User>> &Model::getActiveClientsForDocument() {
    return this->activeClientsForDocument;
}

void Model::save(CrdtMessage crdtMessage) {
    QVector<Symbol> symbols = this->editor->serverProcess(this->getSymbolsForDocument(crdtMessage.getFileName()),
                                                          crdtMessage);
    auto it = symbolsForDocument.find(crdtMessage.getFileName());
    if (it != symbolsForDocument.end())
        it->second = symbols;

    QRegExp tagExp("/");
    QStringList dataList = crdtMessage.getFileName().split(tagExp);

    QString filenamePath = QDir(
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE).append(
                    dataList[0]) + "/").filePath(dataList[1]);
    QFile file(filenamePath);
    if (!file.open(QFile::WriteOnly))
        return;

    QDataStream out(&file);
    out << symbolsForDocument[crdtMessage.getFileName()];
    file.close();
}