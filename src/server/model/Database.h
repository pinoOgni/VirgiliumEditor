//
// Created by pinoOgni on 26/08/20.
//

#ifndef VIRGILIUM_DATABASE_H
#define VIRGILIUM_DATABASE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QList>
#include "../../common/constants.h"
#include <QMessageBox>
#include <QObject>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QFileInfo>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QCryptographicHash>
#include <common/messages/InvitationMessage.h>
#include <common/messages/StorageMessage.h>
#include "common/messages/FilesMessage.h"
#include "common/constants.h"
#include "common/messages/FileManagementMessage.h"
#include "common/messages/UserManagementMessage.h"
#include "common/messages/ChangePasswordMessage.h"
#include "common/messages/UserMessage.h"
#include "common/User.h"

class Database {

private:
    const std::string databaseName = "database.db";
    QSqlDatabase db;

    Database();

    /*
     * This method is used to init the database
     */
    void initDatabase();

    /*
     * This metthod is used to create initial tables
     */
    void createTables(QString, QSqlDatabase);

    /*
     * This methos is used to create put some data in the tables
     * like users, files and so on
     */
    void fillTablesForTests(QString, QSqlDatabase);


    static void printErrorAndExit(const std::string &error = "Unable to connect to DB");

public:
    Database(const Database &) = delete;

    void operator=(const Database &) = delete;

    // return the singleton DB instance
    static Database &getInstance() {
        static Database DBinstance;
        return DBinstance;
    }

    /*
     * serve per controllare se le credenziali di un utente che sta cercando
     * di loggarsi siano corrette
     * @return true if true, false otherwise
     */
    bool loginDB(User);

    /*
     * it is used to register a user, checking if there is already a user with that email
     * @return true if true, false otherwise
     */
    bool signinDB(User);

    /*
     * the User returned is made up of email, firstName, lastName to be displayed on the user's personal page
     * @return User
     */
    User getInfoUser(User);

    /*
     * returns the files owned by the User, together with the collaborators
     * the last access of the User owner and the name of the file
     */
    std::vector<FilesMessage> getFilesOwner(User);

    /*
     * returns the files to which the User collaborates, with his last access
     */
    std::vector<FilesMessage> getUserFiles(User);

    /*
     * it is used to rename a file in the DB, checking that the parameters are correct
     * @return true if true, false otherwise
     */
    bool renameFileDB(FileManagementMessage);

    /*
     * it is used to delete a file in the DB, checking that the parameters are correct
     * @return true if true, false otherwise
     */
    bool deleteFileDB(FileManagementMessage);

    /*
    * it is used to create a file in the DB, checking that the parameters are correct
    * @return true if true, false otherwise
    */
    bool newFileDB(FileManagementMessage);

    /*
    * it is used to change a user's password by checking that the parameters are correct
    * @return true if true, false otherwise
    */
    bool changePasswordDB(const ChangePasswordMessage &);

    /*
     * used to remove a collaborator from a file owned by the client User
     * if the new collaborator exists, if he is really a collaborator also check the password
     * @return true if true, false otherwise
     */
    bool removeCollaboratorDB(const UserManagementMessage &);

    /*
    * used to remove the client requesting it from a file
    * it collaborates with check if the client actually collaborates with that file
    * (maybe it has already been removed) also check the password
    * @return true if true, false otherwise
    */
    bool unsubscribeDB(const UserManagementMessage &);

    /*
     * closes the database, ie the connection with the DB
     * @return void
     */
    void closeConnectionDB();


    /*
     * is used to create a URL that the user will send through third parties to another user adds it to the DB in the invitation_url table
     * @return URL if okay, empty string otherwise
     */
    QString createUrlCollaboratorDB(const UserManagementMessage &);

    /*
     * is used to add a user to a file, thanks to an invitation code
     * @return true if true, false otherwise
     */
    bool requestToCollaborateDB(const InvitationMessage &invitationMessage);

    /*
     * it is used to update the last access to a file
     * @return true if true, false otherwise
    */
    bool updateLastAccessDB(QString email, _int idFilename);

    /*
     * used to return an id given a user and a filename, useful for active users and for updating the last access
     * return id if ok, 0 otherwise
     */
    _int getIdFilenameDB(QString email_owner, QString filename);

    /*
     * is to see if a user can access a file, check the user_files table
     * return true if it can access, false otherwise
     */
    bool canOpenFileDB(const UserManagementMessage &userManagementMessage);


};

#endif // VIRGILIUM_DATABASE_H

