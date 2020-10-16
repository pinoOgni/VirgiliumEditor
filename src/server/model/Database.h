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

  	void initDatabase();

  	bool openConnection(); //TODO

	void createTables(QString,QSqlDatabase);
    void fillTablesForTests(QString,QSqlDatabase);


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
      * @return true se va bene, false altrimenti
      */
     bool loginDB(User);
     /*
      * serve per iscrivere un utente, controllando se non esiste già un utente
      * con quella email
      * @return true se va bene, false altrimenti
      */
     bool signinDB(User);
     /*
      * lo User ritornato è fatto da email, firstName, lastName
      * da far visualizzare nella pagina personale dell'utente
      * @return User
      */
     User getInfoUser(User);
     /*
      * ritorna i file di proprietà dello User, insieme ai collaboratori
      * l'ultimo accesso dello User owner e il nome del file
      */
     std::vector<FilesMessage> getFilesOwner(User);

     /*
      * ritorna i file a cui collabora lo User, con il suo ultimo accesso
      *
      */
     std::vector<FilesMessage> getUserFiles(User);

     /*
      * serve per rinominare un file nel DB, controllando che i parametri siano corretti
      * @return true se va bene, altrimenti false
      */
     bool renameFileDB(FileManagementMessage);
    /*
     * serve per eliminare un file nel DB, controllando che i parametri siano corretti
     * @return true se va bene, altrimenti false
     */
     bool deleteFileDB(FileManagementMessage);
     /*
     * serve per creare un file nel DB, controllando che i parametri siano corretti
     * @return true se va bene, altrimenti false
     */
     bool newFileDB(FileManagementMessage);
     /*
     * serve per cambiare la password di un utente controllando che i parametri siano corretti
     * @return true se va bene, altrimenti false
     */
     bool changePasswordDB(const ChangePasswordMessage&);
     /*
      * serve per aggiungere un collaboratore ad un file di proprietà del client User
      * controlla se il nuovo collaboratore esiste, se è già un collaboratore
      * controlla anche la password
      * @return true se va bene, false altrimenti
      */
     bool addCollaboratorDB(UserManagementMessage);
     /*
     * serve per rimuovere un collaboratore ad un file di proprietà del client User
     * controlla se il nuovo collaboratore esiste, se è davvero un collaboratore
     * controlla anche la password
     * @return true se va bene, false altrimenti
     */
     bool removeCollaboratorDB(const UserManagementMessage&);
     /*
     * serve per rimuovere il client che ne fa richiesta da un file a cui collabora
     * controlla se in effetti il client collabora a quel file (magari è già stato rimosso)
     * controlla anche la password
     * @return true se va bene, false altrimenti
     */
     bool unsubscribeDB(const UserManagementMessage&);

     /*
      * chiude il database, ossia la connessione con il DB
      * quando lo User clicca su logout
      * @return void
      */
     void closeConnectionDB();


     /*
     * serve a creare un URL che lo user invierà attraverso terze parti ad un altro user
     * lo aggiunge al DB nella tabella invitation_url
     * @return l'URL se va bene, empty string altrimenti
     */
     QString createUrlCollaboratorDB(const UserManagementMessage&);

     /*
     * serve, per aggiungere uno user ad un file, grazie ad un codice di invito
     * @return true se va bene, false altrimenti
     */
     bool requestToCollaborateDB(const InvitationMessage& invitationMessage);

     /*
     * serve per aggiornare il last access ad un file
     * return true se va bene, false altrimenti
     */
     bool updateLastAccessDB(QString email, _int idFilename);

     /*
     * serve per ritoranre un id dato un utente e un filename, utile per gli utenti attivi e per l'aggiornamento del last access
     * return id se va bene, 0 altrimenti
     */
     _int getIdFilenameDB(QString email_owner, QString filename);

     /* serve per vedere se un utente può accedere ad un file, si controlla la tabella user_files
      * dentro storageMessage c'è:
      *     email_owner/filename dentro fileName
      *     email dell'utente che fa richiesta dentro storageMessage.getActiveUsers().at(0).getEmail();
      * return true se può accedere, false, altrimenti
      */
     bool canOpenFileDB(const UserManagementMessage& userManagementMessage);


};

#endif // VIRGILIUM_DATABASE_H

