//
// Created by pinoOgni on 26/09/20.
//

#ifndef VIRGILIUM_MODEL_H
#define VIRGILIUM_MODEL_H

#include <QString>
#include <map>
#include <common/ClientSocket.h>
#include "ServerDocument.h"
#include <atomic>
#include <memory>
#include <QtCore/QMutex>
#include <common/messages/UserMessage.h>
#include <common/User.h>
#include "Database.h"
#include <QtSql/QSqlDatabase>
#include <QtCore/QQueue>
#include <common/CRDT/Crdt_editor.h>

class Model {

    //std::multimap<ServerDocument *, ClientSocket *> fileToClients;

    std::map<_int, ClientSocket *> clientToUser;
    std::map<QString, QList<User>> activeClientsForDocument;
    QMutex symbolsForDocumentMutex;
    std::map<QString, QVector<Symbol>> symbolsForDocument;
    std::atomic<quint32> IDSeed;
    QQueue<CrdtMessage> messages;
    QMutex editorMutex;
    Crdt_editor *editor;
    QMutex fileMutex;
public:
    Model();

    /* Manage clientToUser */
    void insertLoggedUser(ClientSocket *socket, const User &user);

    void removeLoggedUser(ClientSocket *socket);

    ClientSocket *getLoggedUser(User &user);

    /* Manage activeClientsForDocument */
    QList<User> addActiveUser(const User &user, const QString &fileName);

    QList<User> removeActiveUser(const User &user, const QString &fileName);

    void removeActiveUser(_int siteId);

    std::map<QString, QList<User>> &getActiveClientsForDocument();

    /* Manage messages */
    void insertMessage(const CrdtMessage &message);

    QQueue<CrdtMessage> &getMessages();

    /* Manage symbolsForDocument */
    void insertSymbolsForDocument(const QString &fileName, const QVector<Symbol> &symbols);

    void removeSymbolsForDocument(const QString &fileName);

    QVector<Symbol> getSymbolsForDocument(const QString &fileName);

    //void removeUserFromEditor(ClientSocket *socket); //serve?
    //void insertUserIntoEditor(ServerDocument *, ClientSocket *); //serve?
    static bool loginUser(User user);

    static bool signinUser(User user);

    User getInfoUser(User user);

    std::vector<FilesMessage> getFilesOwner(User user);

    std::vector<FilesMessage> getUserFiles(User user);

    //void getAllData(User user, User &userReturn,  std::vector<FilesMessage> filesOwner,  std::vector<FilesMessage> filesCollabs);
    bool renameFile(FileManagementMessage fileManagementMessage);

    bool deleteFile(FileManagementMessage fileManagementMessage);

    bool newFile(FileManagementMessage fileManagementMessage);

    bool changePassword(ChangePasswordMessage changePasswordMessage);

    bool addCollaborator(UserManagementMessage userManagementMessage);

    bool removeCollaborator(UserManagementMessage userManagementMessage);

    bool unsubscribe(UserManagementMessage userManagementMessage);

    void closeConnectionDB();

    QString createUrlCollaborator(UserManagementMessage userManagementMessage);

    bool requestToCollaborate(InvitationMessage invitationMessage);

    void save(CrdtMessage crdtMessage);

    QVector<Symbol> performServerProcess(QVector<Symbol> symbols, CrdtMessage crdtMessage);

    void updateSymbolsForDocument(QString filename, QVector<Symbol> toBeSaved);

    QVector<Symbol> getFileFromFileSystem(QString filename);


    //pino 12 ottobre
    _int getIdFilename(QString email_owner, QString filename);
    bool updateLastAcces(QString email, _int idFilename);
};

#endif // VIRGILIUM_MODEL_H

