//
// Created by pinoOgni on 26/09/20.
//

#include "Model.h"
#include "SaveFileService.h"

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

    if (it != clientToUser.end())
        clientToUser.erase(it);
}

bool Model::isUserOnline(const QString email) {
    //spdlog::debug("isUserOnline");
    bool find = false;
    for (auto it = onlineUsers.begin(); it != onlineUsers.end(); ++it) {
        if (QString::compare(*it,email)==0) {
            find = true;
        }
    }
    return find;
}

void Model::insertUserOnline(const QString email) {
    onlineUsers.emplace_back(email);
    //spdlog::debug("insertUserOnline");
}

void Model::removeUserOnline(const QString email) {
    for (auto it = onlineUsers.begin(); it != onlineUsers.end(); ++it) {
        if (QString::compare(*it,email)==0) {
            it = onlineUsers.erase(it);
            break;
        }
    }
}

ClientSocket *Model::getLoggedUser(const User &user) {
    auto it = clientToUser.find(user.getSiteId());
    if (it == clientToUser.end()) {
        throw std::runtime_error("User not logged!");
    } else {
        return it->second;
    }
}

void Model::insertSymbolsForDocument(const QString &fileName, const QVector<Symbol> &symbols) {
    QMutexLocker lock(&this->symbolsForDocumentMutex);
    symbolsForDocument.insert(std::pair<QString, QVector<Symbol>>(fileName, symbols));
}

void Model::removeSymbolsForDocument(const QString &fileName) {
    QMutexLocker lock(&this->symbolsForDocumentMutex);
    symbolsForDocument.erase(fileName);
}

QVector<Symbol> Model::getSymbolsForDocument(const QString &fileName) {
    QMutexLocker lock(&this->symbolsForDocumentMutex);
    auto it = symbolsForDocument.find(fileName);
    if (it == symbolsForDocument.end())
        throw std::runtime_error("Document not opened!");

    return symbolsForDocument[fileName];
}

QList<User> Model::addActiveUserForDocument(const User &user, const QString &fileName) {
    //fileName is email_owner/filename
    QRegExp tagExp("/");
    QStringList firstList = fileName.split(tagExp);
    QString email_owner = firstList.at(0);
    QString filename = firstList.at(1);
    _int idFilename = getIdFilename(email_owner,filename);

    auto it = activeClientsForDocument.find(idFilename);
    if (it != activeClientsForDocument.end()) { /* file already opened */
        activeClientsForDocument.at(idFilename).push_back(user);
        return activeClientsForDocument.at(idFilename);
    } else {
        QList<User> users = {user};
        activeClientsForDocument.insert(std::pair<_int, QList<User>>(idFilename, users));
        return users;
    }
}

QList<User> Model::removeActiveUserForDocument(const User &user, const QString &fileName) {
    //fileName is email_owner/filename
    QRegExp tagExp("/");
    QStringList firstList = fileName.split(tagExp);
    QString email_owner = firstList.at(0);
    QString filename = firstList.at(1);
    _int idFilename = getIdFilename(email_owner,filename);

    auto it = activeClientsForDocument.find(idFilename);
    if (it == activeClientsForDocument.end())
        return QList<User>();

    activeClientsForDocument.at(idFilename).removeOne(user);
    return activeClientsForDocument[idFilename];
}

void Model::removeActiveUser(_int siteId) {

    _int idFilename = 0;

    for (auto &it : this->activeClientsForDocument) {
        for (auto it1 = it.second.begin(); it1 != it.second.end(); it1++) {
            if (it1->getSiteId() == siteId) {
                idFilename = it.first;
                it.second.erase(it1);
                break;
            }
        }
    }

    if (idFilename != 0 && activeClientsForDocument.at(idFilename).empty())
        activeClientsForDocument.erase(idFilename);
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

std::map<_int, QList<User>> &Model::getActiveClientsForDocument() {
    return this->activeClientsForDocument;
}

void Model::save(CrdtMessage crdtMessage) {
    auto symbols = this->getSymbolsForDocument(crdtMessage.getFileName());
    auto toBeSaved = this->performServerProcess(symbols, crdtMessage);

    QMutexLocker lockSymbols(&this->symbolsForDocumentMutex);
    auto it = symbolsForDocument.find(crdtMessage.getFileName());
    if (it != symbolsForDocument.end())
        it->second = toBeSaved;
    lockSymbols.unlock();

    auto *saveFileService = new SaveFileService(*this, std::move(crdtMessage), toBeSaved);
    QThreadPool::globalInstance()->start(saveFileService);
}

QVector<Symbol> Model::performServerProcess(QVector<Symbol> symbols, const CrdtMessage &crdtMessage) {
    QMutexLocker lock(&this->editorMutex);
    return this->editor->serverProcess(std::move(symbols),
                                       crdtMessage);
}

void Model::updateSymbolsForDocument(const QString &filename, const QVector<Symbol> &toBeSaved) {
    QMutexLocker fileLock(&this->fileMutex);

    QRegExp tagExp("/");
    QStringList dataList = filename.split(tagExp);

    QString filenamePath = QDir(
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE).append(
                    dataList[0]) + "/").filePath(dataList[1]);
    QFile file(filenamePath);
    if (!file.open(QFile::WriteOnly))
        return;

    QDataStream out(&file);
    out << toBeSaved;
    file.close();
}

QVector<Symbol> Model::getFileFromFileSystem(const QString &filename) {
    QVector<Symbol> symbols;
    QMutexLocker fileLock(&this->fileMutex);
    QFile file(QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(
            VIRGILIUM_STORAGE)).filePath(filename));
    if (!file.exists())
        return symbols;

    if (!file.open(QFile::ReadOnly))
        return symbols;

    QDataStream in(&file);
    in >> symbols;
    file.close();
    return symbols;
}


_int Model::getIdFilename(QString email_owner, QString filename) {
    return Database::getInstance().getIdFilenameDB(email_owner,filename);
}


bool Model::updateLastAcces(QString email, _int idFilename) {
    return Database::getInstance().updateLastAccessDB(email,idFilename);
}


bool Model::canOpenFile(UserManagementMessage userManagementMessage) {
    return Database::getInstance().canOpenFileDB(userManagementMessage);
}


