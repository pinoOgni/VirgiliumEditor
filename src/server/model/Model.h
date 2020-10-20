//
// Created by pinoOgni on 26/09/20.
//

#ifndef VIRGILIUM_MODEL_H
#define VIRGILIUM_MODEL_H

#include <QString>
#include <map>
#include <common/ClientSocket.h>
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
    /*
     * This map is used to keep the ClientSocket reference of each logged user.
     */
    std::map<_int, ClientSocket *> clientToUser;
    /*
     * This map is used to keep the email of each logged user.
     */
    std::map<_int, QString> onlineUsers;
    /*
     * This map is used to keep the current active user for each document.
     */
    std::map<_int, QList<User>> activeClientsForDocument;
    /*
     * This map is used to keep the list of symbols for each document.
     */
    std::map<QString, QVector<Symbol>> symbolsForDocument;
    std::atomic<quint32> IDSeed;
    QQueue<CrdtMessage> messages;
    Crdt_editor *editor;
    QMutex fileMutex;
public:
    Model();

    /*
     * After the login of a client, a new pair is inserted in order to keep the ClientSocket reference.
     */
    void insertLoggedUser(ClientSocket *socket, const User &user);

    /*
     * After the logout of a client, the pair is removed.
     */
    void removeLoggedUser(ClientSocket *socket);

    /*
     * This method is used to get the ClientSocket reference related to the passed user.
     */
    ClientSocket *getLoggedUser(const User &user);

    bool isUserOnline(const QString &email);

    void insertUserOnline(_int, const QString email);

    void removeUserOnline(_int);

    /*
     * When the user open the editor, if the document is already opened he/she is inserted inside the
     * list of active clients, while if the document is not opened a new pair is create.
     */
    QList<User> addActiveUserForDocument(const User &user, const QString &fileName);

    /*
     * When the user close the editor, if there are other active clients for this document he/she is
     * just deleted from the list, otherwise the pair is removed.
     */
    QList<User> removeActiveUserForDocument(const User &user, const QString &fileName);

    /*
     * This method has the same aim of the previous one, the only difference are the parameters.
     */
    void removeActiveUser(_int siteId);

    /*
     * This method is used to get the reference of the activeClientForDocument map.
     */
    std::map<_int, QList<User>> &getActiveClientsForDocument();

    /*
     * This method is used to insert a new message inside the queue.
     */
    void insertMessage(const CrdtMessage &message);

    /*
     * This method is used to get the message queue.
     */
    QQueue<CrdtMessage> &getMessages();

    /*
     * This method is used to insert the vector of symbols inside the map.
     */
    void insertSymbolsForDocument(const QString &fileName, const QVector<Symbol> &symbols);

    /*
     * This method is used to remove the vector of symbols inside the map.
     */
    void removeSymbolsForDocument(const QString &fileName);

    /*
     * This method is used to get the vector of symbols inside the map.
     */
    QVector<Symbol> getSymbolsForDocument(const QString &fileName);

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

    /*
     * In this method the new symbol is inserted in the correct position inside the vector.
     * After that a pool thread is used in order to save the list of symbols inside the file system.
     */
    void save(CrdtMessage crdtMessage);

    /*
     * This method is invoked by the save method in order to insert the symbol inside the vector
     * in the correct position.
     */
    QVector<Symbol> performServerProcess(QVector<Symbol> symbols, const CrdtMessage &crdtMessage);

    /*
     * This method is used by the threads in order to actually save the list of symbols in the file system.
     */
    void updateSymbolsForDocument(const QString &filename, const QVector<Symbol> &toBeSaved);

    /*
     * This method is used in order to get the list of symbols from the file system.
     */
    QVector<Symbol> getFileFromFileSystem(const QString &filename);

    _int getIdFilename(QString email_owner, QString filename);

    bool updateLastAccess(QString email, _int idFilename);

    /*
     * When the user try to open a new editor this method is called in order to check if there are not
     * other open editors and if the user is a collaborator for the document.
     */
    bool canOpenFile(const UserManagementMessage &userManagementMessage);

};

#endif // VIRGILIUM_MODEL_H

