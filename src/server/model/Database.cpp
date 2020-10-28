//
// Created by pinoOgni on 26/08/20.
//

#include <QtSql/QSqlQuery>
#include <QtCore/QDateTime>
#include "Database.h"

Database::Database() {
    if (!db.open())
        initDatabase();
}

void Database::closeConnectionDB() {
    db.close();
}

void Database::initDatabase() {
    //delete databaase file only for testing
    QString dbPath = QDir(
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE)).filePath(
            "database.db");

    //spdlog::debug("init database TESTDB ");
    if (TESTDB) {
        QFile file(dbPath);
        file.open(QIODevice::WriteOnly | QIODevice::Text);  //create file
    }

    db = QSqlDatabase::addDatabase("QSQLITE"); //"SQLITE"
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        //spdlog::error("error first try to open DB");
        std::cerr << "error first try to open DB" << std::endl;
    } else {
        db.close();
        if (TESTDB) {
            createTables(dbPath, db);
            fillTablesForTests(dbPath, db);
        }
    }

}


void Database::createTables(QString dbPath, QSqlDatabase db) {
    QFileInfo checkfile(dbPath);
    if (checkfile.isFile()) {
        if (db.open()) {
            QSqlQuery qry;
            //spdlog::debug("createTables correct opened db");

            if (!db.tables().contains("users")) {
                QString create_users = "CREATE TABLE users ("
                                       "email VARCHAR(50) PRIMARY KEY,"
                                       "firstName VARCHAR(20),"
                                       "lastName VARCHAR(20),"
                                       "password VARCHAR(255))";
                if (!qry.exec(create_users)) {
                    //spdlog::error("error create table users");
                    std::cerr << "error create table users" << std::endl;
                }
                QString create_files = "CREATE TABLE files ("
                                       "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                                       "filename VARCHAR(50),"
                                       "email_owner VARCHAR(50))";
                if (!qry.exec(create_files)) {
                    //spdlog::error("error create table files");
                    std::cerr << "error create table files" << std::endl;
                }
                QString create_user_files = "CREATE TABLE user_files ("
                                            "id INTEGER,"
                                            "email VARCHAR(50),"
                                            "last_access DATETIME NOT NULL,"
                                            "PRIMARY KEY (id,email))";
                if (!qry.exec(create_user_files)) {
                    //spdlog::error("error create table files");
                    std::cerr << "error create table files" << std::endl;
                }
                QString create_invitation_urls = "CREATE TABLE invitation_urls ("
                                                 "url VARCHAR(255),"
                                                 "id INTEGER NOT NULL,"
                                                 "timeout DATETIME NOT NULL,"
                                                 "PRIMARY KEY (url))";
                if (!qry.exec(create_invitation_urls)) {
                    //spdlog::error("error create table invitation_urls");
                    std::cerr << "error create table invitation_urls" << std::endl;
                }
                QSqlDatabase::database().commit();
            }
            db.close();
        }
    } else {
        //spdlog::error("createTables error opened db");
        std::cerr << "createTables error opened db" << std::endl;
    }
}


void Database::fillTablesForTests(QString dbPath, QSqlDatabase db) {

    QFileInfo checkfile(dbPath);
    QDateTime dateTime;
    if (checkfile.isFile()) {
        if (db.open()) {
            QSqlQuery qry;
            //spdlog::debug("fillTablesForTests correct opened db");

            QSqlDatabase::database().transaction();
            qry.prepare("INSERT INTO users (email,firstName,lastName,password) VALUES(?,?,?,?)");
            qry.bindValue(0, "pino@pino.com");
            qry.bindValue(1, "GIUSEPPE");
            qry.bindValue(2, "OGNIBENE");
            qry.bindValue(3, QCryptographicHash::hash("pino", QCryptographicHash::Sha224));
            if (!qry.exec()) {
                //spdlog::error("error insert user");
                std::cerr << "error insert user" << std::endl;
            }
            qry.prepare("INSERT INTO users (email,firstName,lastName,password) VALUES(?,?,?,?)");
            qry.bindValue(0, "ale@ale.com");
            qry.bindValue(1, "ALESSANDRO");
            qry.bindValue(2, "PAGANO");
            qry.bindValue(3, QCryptographicHash::hash("ale", QCryptographicHash::Sha224));
            if (!qry.exec()) {
                //spdlog::error("error insert user");
                std::cerr << "error insert user" << std::endl;
            }
            QSqlDatabase::database().commit();
            qry.prepare("INSERT INTO users (email,firstName,lastName,password) VALUES(?,?,?,?)");
            qry.bindValue(0, "simo@simo.com");
            qry.bindValue(1, "SIMONE");
            qry.bindValue(2, "D'AMILO");
            qry.bindValue(3, QCryptographicHash::hash("simo", QCryptographicHash::Sha224));
            if (!qry.exec()) {
                //spdlog::error("error insert user");
                std::cerr << "error insert user" << std::endl;
            }
            QSqlDatabase::database().commit();
            qry.prepare("INSERT INTO users (email,firstName,lastName,password) VALUES(?,?,?,?)");
            qry.bindValue(0, "ste@ste.com");
            qry.bindValue(1, "STEFANO");
            qry.bindValue(2, "SURACI");
            qry.bindValue(3, QCryptographicHash::hash("ste", QCryptographicHash::Sha224));
            if (!qry.exec()) {
                //spdlog::error("error insert user");
                std::cerr << "error insert user" << std::endl;
            }
            QSqlDatabase::database().commit();



            //insert item in files then in user_files, retrieve the last id inserted

            QSqlDatabase::database().transaction();
            qry.prepare("INSERT INTO files(filename, email_owner) VALUES(?,?)");
            QString filename = "telegram";
            qry.bindValue(0, filename);
            qry.bindValue(1, "pino@pino.com");
            if (!qry.exec()) {
                //spdlog::error("error insert file");
                std::cerr << "error insert file" << std::endl;
            }


            //create directory for user pino@pino.com
            QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE)).mkdir(
                    "pino@pino.com");

            //create "telegram"
            QString filenamePath;
            filenamePath = QDir(
                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE).append(
                            "pino@pino.com")).filePath("telegram");
            QFile file(filenamePath);
            file.open(QIODevice::WriteOnly | QIODevice::Text);

            //add ale@ale.com and ste@ste.com to "telegram"
            qry.prepare("SELECT LAST_INSERT_ROWID();");
            if (!qry.exec()) {
                //spdlog::error("error retrieve last id inserted in files");
                std::cerr << "error retrieve last id inserted in files" << std::endl;
            }
            qry.first();
            int id = qry.value(0).toInt();

            //insert the owner
            qry.prepare("INSERT INTO user_files(id, email, last_access) VALUES(?,?,?)");
            qry.bindValue(0, id);
            qry.bindValue(1, "pino@pino.com");
            dateTime = dateTime.currentDateTime();
            qry.bindValue(2, dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
            if (!qry.exec()) {
                //spdlog::error("error insert in user_files");
                std::cerr << "error insert in user_files" << std::endl;
            }

            //insert some collaborators
            qry.prepare("INSERT INTO user_files(id, email, last_access) VALUES(?,?,?)");
            qry.bindValue(0, id);
            qry.bindValue(1, "ale@ale.com");
            dateTime = dateTime.currentDateTime();
            qry.bindValue(2, dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
            if (!qry.exec()) {
                //spdlog::error("error insert in user_files");
                std::cerr << "error insert in user_files" << std::endl;
            }

            qry.prepare("INSERT INTO user_files(id, email, last_access) VALUES(?,?,?)");
            qry.bindValue(0, id);
            qry.bindValue(1, "ste@ste.com");
            dateTime = dateTime.currentDateTime();
            qry.bindValue(2, dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
            if (!qry.exec()) {
                //spdlog::error("error insert in user_files");
                std::cerr << "error insert in user_files" << std::endl;
            }


            //altra prova
            qry.prepare("INSERT INTO files(filename, email_owner) VALUES(?,?)");
            filename = "system programming";
            qry.bindValue(0, filename);
            qry.bindValue(1, "pino@pino.com");
            if (!qry.exec()) {
                //spdlog::error("error insert in files");
                std::cerr << "error insert in files" << std::endl;
            }

            //create "system programming"
            filenamePath = QDir(
                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE).append(
                            "pino@pino.com")).filePath("system programming");
            QFile file2(filenamePath);
            file2.open(QIODevice::WriteOnly | QIODevice::Text);

            //add ale@ale.com and ste@ste.com to "telegram"
            qry.prepare("SELECT LAST_INSERT_ROWID();");
            if (!qry.exec()) {
                //spdlog::error("error retrieve last id inserted in files");
                std::cerr << "error retrieve last id inserted in files" << std::endl;
            }
            qry.first();
            id = qry.value(0).toInt();

            //insert the owner
            qry.prepare("INSERT INTO user_files(id, email, last_access) VALUES(?,?,?)");
            qry.bindValue(0, id);
            qry.bindValue(1, "pino@pino.com");
            dateTime = dateTime.currentDateTime();
            qry.bindValue(2, dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
            if (!qry.exec()) {
                //spdlog::error("error insert in user_files");
                std::cerr << "error insert in user_files" << std::endl;
            }


            //insert some collaborators
            qry.prepare("INSERT INTO user_files(id, email, last_access) VALUES(?,?,?)");
            qry.bindValue(0, id);
            qry.bindValue(1, "ale@ale.com");
            dateTime = dateTime.currentDateTime();
            qry.bindValue(2, dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
            if (!qry.exec()) {
                //spdlog::error("error insert in user_files");
                std::cerr << "error insert in user_files" << std::endl;
            }

            qry.prepare("INSERT INTO user_files(id, email, last_access) VALUES(?,?,?)");
            qry.bindValue(0, id);
            qry.bindValue(1, "simo@simo.com");
            dateTime = dateTime.currentDateTime();
            qry.bindValue(2, dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
            if (!qry.exec()) {
                //spdlog::error("error insert in user_files");
                std::cerr << "error insert in user_files" << std::endl;
            }
            QSqlDatabase::database().commit();
        }
        db.close();
    } else {
        //spdlog::error("fillTablesForTests error opened db");
        std::cerr << "fillTablesForTests error opened db" << std::endl;
    }

}


void Database::printErrorAndExit(const std::string &error) {
    std::cout << error;
    exit(-1);
}


bool Database::loginDB(User user) {

    if (db.open()) {
        //spdlog::debug("loginDB correct opened db to control password");
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT password FROM users WHERE email = :email");
        qry.bindValue(":email", user.getEmail());
        qry.exec();
        qry.first();
        QString psw = qry.value(0).toString();
        QSqlDatabase::database().commit();
        db.close();
        if (QString::compare(psw, QCryptographicHash::hash(user.getPassword().toUtf8(), QCryptographicHash::Sha224)) ==
            0) {
            return true;
        } else {
            return false;
        }
    } else {
        //spdlog::error("loginDB error opened db");
        std::cerr << "loginDB error opened db" << std::endl;
        return false;
    }
}

bool Database::signinDB(User user) {

    if (db.open()) {
        //spdlog::debug("signinDB correct opened db to control user already in db");
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT email FROM users WHERE email = :email");
        qry.bindValue(":email", user.getEmail());
        qry.exec();
        qry.first();
        QString emailDB = qry.value(0).toString();
        QSqlDatabase::database().commit();

        if (emailDB.isNull()) {
            QSqlDatabase::database().transaction();
            qry.prepare("INSERT INTO users (email,firstName,lastName,password) VALUES(?,?,?,?)");
            qry.bindValue(0, user.getEmail());
            qry.bindValue(1, user.getFirstName());
            qry.bindValue(2, user.getLastName());
            qry.bindValue(3, QCryptographicHash::hash(user.getPassword().toUtf8(), QCryptographicHash::Sha224));
            if (!qry.exec()) {
                //spdlog::error("error insert in users");
                std::cerr << "error insert in users" << std::endl;
            }
            QSqlDatabase::database().commit();
            db.close();
            return true;
        } else {
            db.close();
            return false;
        }
    } else {
        //spdlog::error("signinDB error opened db");
        std::cerr << "signinDB error opened db" << std::endl;
        return false;
    }
}


User Database::getInfoUser(User user) {
    User userReturn;
    if (db.open()) {
        //spdlog::debug("getInfoUser correct opened db to control password");
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT email,firstName,lastName FROM users WHERE email = :email");
        qry.bindValue(":email", user.getEmail());
        qry.exec();
        qry.next();

        userReturn = User(user.getEmail(), "psw", qry.value(1).toString(), qry.value(2).toString());

        QSqlDatabase::database().commit();
        db.close();
        return userReturn;

    } else {
        //spdlog::error("getInfoUser error opened db");
        std::cerr << "getInfoUser error opened db" << std::endl;
        userReturn = User();
        return userReturn;
    }
}

std::vector<FilesMessage> Database::getFilesOwner(User user) {
    std::vector<FilesMessage> filesMessage;
    if (db.open()) {
        QSqlDatabase::database().transaction();
        QSqlQuery qry, qry1, qry0;


        qry.prepare("SELECT count(id) FROM files where email_owner = :email_owner");
        qry.bindValue(":email_owner", user.getEmail());
        qry.exec();
        qry.first();

        qry.prepare("SELECT filename,id FROM files WHERE email_owner = :email_owner");
        qry.bindValue(":email_owner", user.getEmail());
        qry.exec();

        int row = 0;

        for (qry.first(); qry.isValid(); qry.next(), ++row) {
            int id = qry.value(1).toInt();
            qry0.prepare("SELECT last_access FROM user_files WHERE id = :id and email =:email_owner");
            qry0.bindValue(":id", id);
            qry0.bindValue(":email_owner", user.getEmail());
            qry0.exec();
            qry0.first();
            QString last_access = qry0.value(0).toString();

            qry1.prepare("SELECT email FROM user_files WHERE id = :id AND email <> :email_owner");
            qry1.bindValue(":id", id);
            qry1.bindValue(":email_owner", user.getEmail());
            qry1.exec();
            QStringList collaborators;

            while (qry1.next()) {
                //collaborators.append(qry1.value(0).toString()).append(" ");
                collaborators << qry1.value(0).toString();
            }
            filesMessage.emplace_back(0, qry.value(0).toString(), last_access, collaborators, user.getEmail());
            collaborators.clear();
        }
        QSqlDatabase::database().commit();
        db.close();
        return filesMessage;

    } else {
        //spdlog::error("getFilesOwner, error opened db when try to get user's info");
        std::cerr << "getFilesOwner, error opened db when try to get user's info" << std::endl;
        return filesMessage;
    }
}


std::vector<FilesMessage> Database::getUserFiles(User user) {
    std::vector<FilesMessage> filesMessage;
    if (db.open()) {
        QSqlDatabase::database().transaction();
        QSqlQuery qry, qry1;

        qry.prepare(
                "SELECT files.filename, files.email_owner, user_files.last_access, files.id FROM files, user_files WHERE user_files.id = files.id AND user_files.email = :email");
        // select files.filename, files.email_owner, user_files.last_access, files.id from files, user_files where user_files.id = files.id and user_files.email = "ale@ale.com";
        qry.bindValue(":email", user.getEmail());
        if (!qry.exec()) {
            //spdlog::error("getUserFiles error retrieve file where this user collaborates");
            std::cerr << "getUserFiles error retrieve file where this users collaborates" << std::endl;
        }

        int row = 0;

        //senza QString::compare(qry.value(1).toString(),user.getEmail())!=0; nella seconda tabella spuntano i file di cui l'utente è owner
        for (qry.first(); qry.isValid(); qry.next(), ++row) {

            int id = qry.value(3).toInt();

            if (QString::compare(qry.value(1).toString(), user.getEmail()) != 0) {
                //nella lista dei collaboratori non voglio l'utente owner quindi serve <> :email_owner
                qry1.prepare("SELECT email FROM user_files WHERE id = :id AND email <> :email_owner");
                qry1.bindValue(":id", id);
                qry1.bindValue(":email_owner", qry.value(1).toString());
                qry1.exec();
                QStringList collaborators;

                while (qry1.next()) {
                    //collaborators.append(qry1.value(0).toString()).append(" ");
                    collaborators << qry1.value(0).toString();
                }
                filesMessage.emplace_back(0, qry.value(0).toString(), qry.value(2).toString(), collaborators,
                                          qry.value(1).toString());
                collaborators.clear();
            }
        }
        QSqlDatabase::database().commit();
        db.close();

        for (int i = 0; i < filesMessage.size(); i++)
            filesMessage[i].printUserInfo();

        return filesMessage;
    } else {
        //spdlog::debug("getUserFiles error opened db when try to get user's info");
        return filesMessage;
    }
}


bool Database::renameFileDB(FileManagementMessage fileManagementMessage) {
    QString filenameTemp;
    bool renamed = false;
    if (db.open()) {
        //spdlog::debug("renameFileDB correct open db");
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT password FROM users WHERE email = :email");
        qry.bindValue(":email", fileManagementMessage.getEmail());
        qry.exec();
        qry.first();
        QString psw = qry.value(0).toString();


        QSqlDatabase::database().commit();
        if (QString::compare(psw, fileManagementMessage.getPassword()) == 0) {

            QSqlDatabase::database().transaction();

            QSqlQuery qry;
            QSqlDatabase::database().transaction();
            bool go = true;
            qint8 n = 1;
            filenameTemp = fileManagementMessage.getFilename(); //newFilename
            while (go) {
                qry.prepare("SELECT filename FROM files WHERE email_owner = :email_owner AND filename = :filenameTemp");
                qry.bindValue(":email_owner", fileManagementMessage.getEmail());
                qry.bindValue(":filenameTemp", filenameTemp);
                if (!qry.exec()) {
                    //spdlog::error("error select filename");
                    std::cerr << "error select filename" << std::endl;
                }
                qry.first();
                QString filenameDB = qry.value(0).toString();

                if (filenameDB.isNull()) {
                    //interrupt while loop
                    go = false;

                    qry.prepare(
                            "UPDATE files SET filename = :newFilename WHERE email_owner = :email_owner AND filename = :oldFilename");
                    qry.bindValue(":email_owner", fileManagementMessage.getEmail());
                    qry.bindValue(":oldFilename", fileManagementMessage.getOldfilename());
                    qry.bindValue(":newFilename", filenameTemp);

                    if (!qry.exec()) {
                        //spdlog::error("renameFileDB error UPDATE files");
                        std::cerr << "renameFileDB error UPDATE files" << std::endl;
                        return false;
                    } else {
                        //spdlog::debug("renameFileDB, renamed true");
                        renamed = true;
                    }
                } else {
                    filenameTemp = fileManagementMessage.getFilename();
                    filenameTemp = filenameTemp.append("_").append(QString::number(n));
                    n++;
                }
            }

            QSqlDatabase::database().commit();
            db.close();

            if (renamed) {
                QString filenamePath;
                filenamePath = QDir(
                        QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE).append(
                                fileManagementMessage.getEmail())).filePath(fileManagementMessage.getOldfilename());


                QFile file(filenamePath);
                QString newFilenamePath = QDir(
                        QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE).append(
                                fileManagementMessage.getEmail())).filePath(filenameTemp);

                file.rename(newFilenamePath);
                return true;
            } else
                return false;
        } else {
            db.close();
            return false;
        }
    } else {
        //spdlog::error("renameFileDB error opened db");
        std::cerr << "renameFileDB error opened db" << std::endl;
        return false;
    }
}


bool Database::deleteFileDB(FileManagementMessage fileManagementMessage) {
    if (db.open()) {
        //spdlog::debug("deleteFileDB correct opened db");
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT password FROM users WHERE email = :email");
        qry.bindValue(":email", fileManagementMessage.getEmail());
        qry.exec();
        qry.first();
        QString psw = qry.value(0).toString();
        QSqlDatabase::database().commit();
        if (QString::compare(psw, fileManagementMessage.getPassword()) == 0) {
            QSqlDatabase::database().transaction();

            //take id to delete files in user_files table
            qry.prepare("SELECT id FROM files WHERE email_owner = :email_owner and filename = :filename");
            qry.bindValue(":email_owner", fileManagementMessage.getEmail());
            qry.bindValue(":filename", fileManagementMessage.getFilename());
            if (!qry.exec()) {
                //spdlog::error("error while retrieve id from files table");
                std::cerr << "error while retrieve id from files table" << std::endl;
                return false;
            }

            qry.first();
            int idToDelete = qry.value(0).toInt();

            //delete rows in user_files: i.e. delete collaborators, ALL COLLABORATORS
            if (idToDelete != 0) {
                qry.prepare("DELETE FROM user_files WHERE user_files.id = :idToDelete");
                qry.bindValue(":idToDelete", idToDelete);
                if (!qry.exec()) {
                    //spdlog::error("error while deleting rows from user_files table");
                    std::cerr << "error while deleting rows from user_files table" << std::endl;
                    return false;
                }

                //delete file in files table
                qry.prepare("DELETE FROM files  WHERE files.id = :idToDelete");
                qry.bindValue(":idToDelete", idToDelete);
                if (!qry.exec()) {
                    //spdlog::error("error while deleting file from files table");
                    std::cerr << "error while deleting file from files table" << std::endl;
                    return false;
                }
                QSqlDatabase::database().commit();
                db.close();

                QString filename = QDir(
                        QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE).append(
                                fileManagementMessage.getEmail())).filePath(fileManagementMessage.getFilename());
                QFile file(filename);
                file.remove();
            } else {
                db.close();
                return false;
            }
        } else {
            db.close();
            return false;
        }
    } else {
        //spdlog::error("deleteFileDB error opened db");
        std::cerr << "deleteFileDB error openend db" << std::endl;
        return false;
    }
    return true;
}


bool Database::newFileDB(FileManagementMessage fileManagementMessage) {
    //creation of new file: insert into files, control if there is a equal filename
    QString filenameTemp;
    bool created = false;
    if (db.open()) {
        QSqlQuery qry;
        //spdlog::debug("newFileDB correct opened db");
        bool go = true;
        qint8 n = 1;
        filenameTemp = fileManagementMessage.getFilename();
        while (go) {
            QSqlDatabase::database().transaction();
            qry.prepare("SELECT filename FROM files WHERE email_owner = :email_owner AND filename = :filenameTemp");
            qry.bindValue(":email_owner", fileManagementMessage.getEmail());
            qry.bindValue(":filenameTemp", filenameTemp);
            if (!qry.exec()) {
                //spdlog::error("error select filename");
                std::cerr << "error select filename" << std::endl;
            }
            qry.first();
            QString filenameDB = qry.value(0).toString();
            QSqlDatabase::database().commit();
            if (filenameDB.isNull()) {
                QSqlDatabase::database().transaction();
                //interrupt while loop
                go = false;
                qry.prepare("INSERT INTO files(filename, email_owner) VALUES(?,?)");
                qry.bindValue(0, filenameTemp);
                qry.bindValue(1, fileManagementMessage.getEmail());

                if (!qry.exec()) {
                    //spdlog::error("error insert new file in files tables");
                    std::cerr << "error insert new file in files tables" << std::endl;
                    return false;
                }

                qry.prepare("SELECT LAST_INSERT_ROWID();");
                if (!qry.exec()) {
                    //spdlog::error("error retrieve last id inserted in files");
                    std::cerr << "error retrieve last id inserted in files" << std::endl;
                }
                qry.first();
                int id = qry.value(0).toInt();
                QSqlDatabase::database().commit();
                QSqlDatabase::database().transaction();
                //insert the owner
                qry.prepare("INSERT INTO user_files(id, email, last_access) VALUES(?,?,?)");
                qry.bindValue(0, id);
                qry.bindValue(1, fileManagementMessage.getEmail());
                QDateTime dateTime = dateTime.currentDateTime();
                qry.bindValue(2, dateTime.toString("dd/MM/yyyy  hh:mm:ss"));

                if (!qry.exec()) {
                    //spdlog::error("error insert into user_files");
                    std::cerr << "error insert into user_files" << std::endl;
                } else {
                    created = true;
                }
                QSqlDatabase::database().commit();
            } else {
                filenameTemp = fileManagementMessage.getFilename();
                filenameTemp = filenameTemp.append("_").append(QString::number(n));
                n++;
            }
        }
        QSqlDatabase::database().commit();
        db.close();

        if (created) {
            QString filenamePath;
            if (!QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE).append(
                    fileManagementMessage.getEmail())).exists())
                QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE)).mkdir(
                        fileManagementMessage.getEmail());

            filenamePath = QDir(
                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append(VIRGILIUM_STORAGE).append(
                            fileManagementMessage.getEmail())).filePath(filenameTemp);

            QFile file(filenamePath);
            file.open(QIODevice::WriteOnly | QIODevice::Text);

            return true;
        } else
            return false;
    } else {
        //spdlog::error("newFileDB error opened db");
        std::cerr << "newFileDB error opened db" << std::endl;
        return false;
    }
}


bool Database::changePasswordDB(const ChangePasswordMessage &changePasswordMessage) {
    if (db.open()) {
        //spdlog::debug("correct opened db to control password");
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT password FROM users WHERE email = :email");
        qry.bindValue(":email", changePasswordMessage.getEmail());
        qry.exec();
        qry.first();
        QString psw = qry.value(0).toString();

        if (QString::compare(psw, changePasswordMessage.getOldPsw()) == 0) {
            qry.prepare("UPDATE users SET password = :newPsw WHERE email = :email");
            qry.bindValue(":email", changePasswordMessage.getEmail());
            qry.bindValue(":newPsw", changePasswordMessage.getNewPsw());
            if (!qry.exec()) {
                //spdlog::error("error update password");
                std::cerr << "error update password" << std::endl;
            }
            QSqlDatabase::database().commit();
            db.close();
            return true;
        } else {
            QSqlDatabase::database().commit();
            db.close();
            return false;
        }
    } else {
        //spdlog::error("changePasswordDB error opened db");
        std::cerr << "changePasswordDB error opened db" << std::endl;
        return false;
    }
}

bool Database::requestToCollaborateDB(const InvitationMessage &invitationMessage) {
    if (db.open()) {
        //spdlog::debug("correct opened db to control password");
        QSqlDatabase::database().transaction();
        QSqlQuery qry;

        qry.prepare("SELECT email FROM users WHERE email = :collaborator");
        qry.bindValue(":collaborator", invitationMessage.getEmail());
        qry.exec();
        qry.first();
        QString resUser = qry.value(0).toString();

        qry.prepare("SELECT id FROM invitation_urls WHERE url =:url AND timeout > :timeNow");
        qry.bindValue(":url", invitationMessage.getInvitationCode());
        QDateTime timeNow = timeNow.currentDateTime();
        qry.bindValue(":timeNow", timeNow.toString("dd/MM/yyyy  hh:mm:ss"));
        qry.exec();
        qry.first();
        int idResult = qry.value(0).toInt();

        qry.prepare("SELECT email_owner, filename FROM files WHERE id =:id");
        qry.bindValue(":id", idResult);
        qry.exec();
        qry.first();
        QString email_owner = qry.value(0).toString();
        QString filename = qry.value(1).toString();
        QSqlDatabase::database().commit();

        QString tempURL = QString(
                email_owner + "/" + filename + "/" + idResult + "/" + invitationMessage.getEmail()).toUtf8().toBase64();

        //se l'utente che richiede di collaborare esiste e se l'url che ricevo corrisponde ad un url corretto ossia idResult esiste
        if (!resUser.isEmpty() && QString::compare(tempURL, invitationMessage.getInvitationCode()) == 0) {
            QSqlDatabase::database().transaction();

            qry.prepare("DELETE FROM invitation_urls WHERE url = :url");
            qry.bindValue(":url", invitationMessage.getInvitationCode());
            if (!qry.exec()) {
                //spdlog::error("error deleteting from invitation_urls");
                std::cerr << "error deleting from invitation_urls" << std::endl;
                QSqlDatabase::database().commit();
                db.close();
                return false;
            }

            qry.prepare("INSERT INTO user_files(id, email, last_access) VALUES(?,?,?)");
            qry.bindValue(0, idResult);
            qry.bindValue(1, invitationMessage.getEmail());
            qry.bindValue(2, "dd/MM/yyyy  hh:mm:ss");
            if (!qry.exec()) {
                //spdlog::error("error insert in user_files");
                std::cerr << "error insert in user_files" << std::endl;
                QSqlDatabase::database().commit();
                db.close();
                return false;
            } else {
                QSqlDatabase::database().commit();
                db.close();
                return true;
            }
        } else {
            db.close();
            return false;
        }
    } else {
        //spdlog::error("error opened db");
        std::cerr << "error opened db" << std::endl;
        return false;
    }
}


QString Database::createUrlCollaboratorDB(const UserManagementMessage &userManagementMessage) {
    QString URL = "";
    if (db.open()) {
        //spdlog::debug("correct opened db to control password");
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT password FROM users WHERE email = :email");
        qry.bindValue(":email", userManagementMessage.getEmail_owner());
        if (!qry.exec()) {
            //spdlog::error("error select password in createUrlCollaboratorDB");
            std::cerr << "error select password in createUrlCollaboratorDB" << std::endl;
            QSqlDatabase::database().commit();
            db.close();
            return nullptr;
        }
        qry.first();
        QString psw = qry.value(0).toString();

        qry.prepare("SELECT email FROM users WHERE email = :collaborator");
        qry.bindValue(":collaborator", userManagementMessage.getEmail_collaborator());
        if (!qry.exec()) {
            //spdlog::error("error select email in createUrlCollaboratorDB");
            std::cerr << "error select email in createUrlCollaboratorDB" << std::endl;
            QSqlDatabase::database().commit();
            db.close();
            return nullptr;
        }
        qry.first();
        QString resUser = qry.value(0).toString();

        qry.prepare("SELECT id FROM files WHERE filename =:filename AND email_owner =:email_owner");
        qry.bindValue(":filename", userManagementMessage.getFilename());
        qry.bindValue(":email_owner", userManagementMessage.getEmail_owner());
        if (!qry.exec()) {
            //spdlog::error("error select id in createUrlCollaboratorDB");
            std::cerr << "error select id in createUrlCollaboratorDB" << std::endl;
            QSqlDatabase::database().commit();
            db.close();
            return nullptr;
        }
        qry.first();
        int id = qry.value(0).toInt();

        qry.prepare("SELECT email FROM user_files WHERE id =:id AND email =:email");
        qry.bindValue(":id", id);
        qry.bindValue(":email", userManagementMessage.getEmail_collaborator());
        if (!qry.exec()) {
            //spdlog::error("error email password in createUrlCollaboratorDB");
            std::cerr << "error select email in createUrlCollaboratorDB" << std::endl;
            QSqlDatabase::database().commit();
            db.close();
            return nullptr;
        }
        qry.first();
        QString isAlreadyCollaborator = qry.value(0).toString();
        QSqlDatabase::database().commit();

        //if password is correct, the user that could be added as collaborator is in user table,
        //the file is in the files table and the user that could be as collaborator is not alrdeady a collaborator
        if (QString::compare(psw, userManagementMessage.getPassword()) == 0 && !resUser.isEmpty() && id != 0 &&
            isAlreadyCollaborator.isEmpty()) {
            QSqlDatabase::database().transaction();

            URL = QString(
                    userManagementMessage.getEmail_owner() + "/" + userManagementMessage.getFilename() + "/" + id +
                    "/" + userManagementMessage.getEmail_collaborator()).toUtf8().toBase64();
            qry.prepare("SELECT url FROM invitation_urls WHERE url =:url");
            qry.bindValue(":url", URL);
            qry.exec();
            qry.first();
            QString urlAlreadyExist = qry.value(0).toString();

            //user try to add multiple times the same user, we update te timeout
            if (!urlAlreadyExist.isEmpty()) {
                qry.prepare("UPDATE invitation_urls SET timeout = :timeout WHERE url = :url");
                QDateTime timeout = timeout.currentDateTime();
                timeout = timeout.addSecs(3600);
                qry.bindValue(":timeout", timeout.toString("dd/MM/yyyy  hh:mm:ss"));
                qry.bindValue(":url", URL);
            } else {
                qry.prepare("INSERT INTO invitation_urls(url,id,timeout) VALUES(?,?,?)");
                qry.bindValue(0, URL);
                qry.bindValue(1, id);
                QDateTime timeout = timeout.currentDateTime();
                timeout = timeout.addSecs(3600);
                qry.bindValue(2, timeout.toString("dd/MM/yyyy  hh:mm:ss"));
            }
            if (!qry.exec()) {
                //spdlog::error("error insert in invitation_urls");
                std::cerr << "error insert in invitation_urls" << std::endl;
                QSqlDatabase::database().commit();
                db.close();
                return URL;
            } else {
                QSqlDatabase::database().commit();
                db.close();
                return URL;
            }
        }
    } else {
        //spdlog::error("error opened db");
        std::cerr << "error opened db" << std::endl;
        return URL;
    }
    return URL;
}


bool Database::removeCollaboratorDB(const UserManagementMessage &userManagementMessage) {
    if (db.open()) {
        //spdlog::debug("correct opened db to control password");
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT password FROM users WHERE email = :email");
        qry.bindValue(":email", userManagementMessage.getEmail_owner());
        qry.exec();
        qry.first();
        QString psw = qry.value(0).toString();

        qry.prepare("SELECT email FROM users WHERE email = :collaborator");
        qry.bindValue(":collaborator", userManagementMessage.getEmail_collaborator());
        qry.exec();
        qry.first();
        QString resUser = qry.value(0).toString();

        qry.prepare("SELECT id FROM files WHERE email_owner = :email_owner and filename = :filename");
        qry.bindValue(":email_owner", userManagementMessage.getEmail_owner());
        qry.bindValue(":filename", userManagementMessage.getFilename());
        qry.exec();
        qry.first();
        int id = qry.value(0).toInt();


        qry.prepare("SELECT email FROM user_files WHERE id =:id AND email =:email");
        qry.bindValue(":id", id);
        qry.bindValue(":email", userManagementMessage.getEmail_collaborator());
        qry.exec();
        qry.first();
        QString isAlreadyCollaborator = qry.value(0).toString();
        QSqlDatabase::database().commit();

        //if password is correct, the user that could be removed as collaborator is in user table,
        //the file is in the files table and the user that could be removed as collaborator is a collaborator
        if (QString::compare(psw, userManagementMessage.getPassword()) == 0 && resUser.isEmpty() == false && id != 0 &&
            isAlreadyCollaborator.isEmpty() == false) {
            QSqlDatabase::database().transaction();

            qry.prepare("DELETE FROM user_files WHERE user_files.id = :idToDelete AND user_files.email = :email");
            qry.bindValue(":idToDelete", id);
            qry.bindValue(":email", userManagementMessage.getEmail_collaborator());
            if (!qry.exec()) {
                //spdlog::error("error deleteting from user_files");
                std::cerr << "error deleting from user_files" << std::endl;
                QSqlDatabase::database().commit();
                db.close();
                return false;
            } else {
                //spdlog::debug("collaborator  removed");
                QSqlDatabase::database().commit();
                db.close();
                return true;
            }
        } else {
            db.close();
            return false;
        }
    } else {
        //spdlog::error("error opened db");
        std::cerr << "error opened db" << std::endl;
        return false;
    }
}

bool Database::unsubscribeDB(const UserManagementMessage &userManagementMessage) {
    if (db.open()) {
        //spdlog::debug("correct opened db to control password");
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT password FROM users WHERE email = :email");
        qry.bindValue(":email", userManagementMessage.getEmail_collaborator());
        qry.exec();
        qry.first();
        QString psw = qry.value(0).toString();

        qry.prepare("SELECT id FROM files WHERE filename =:filename AND email_owner =:email_owner");
        qry.bindValue(":filename", userManagementMessage.getFilename());
        qry.bindValue(":email_owner", userManagementMessage.getEmail_owner());
        qry.exec();
        qry.first();
        int id = qry.value(0).toInt();

        qry.prepare("SELECT email FROM user_files WHERE id =:id AND email =:email");
        qry.bindValue(":id", id);
        qry.bindValue(":email", userManagementMessage.getEmail_collaborator());
        qry.exec();
        qry.first();
        QString isCollaborator = qry.value(0).toString();
        QSqlDatabase::database().commit();


        if (QString::compare(psw, userManagementMessage.getPassword()) == 0 && id != 0 &&
            isCollaborator.isEmpty() == false) {
            QSqlDatabase::database().transaction();
            qry.prepare("DELETE FROM user_files WHERE user_files.id = :idToDelete AND user_files.email = :email");
            qry.bindValue(":idToDelete", id);
            qry.bindValue(":email", userManagementMessage.getEmail_collaborator());
            if (!qry.exec()) {
                //spdlog::error("error deleting from user_files");
                std::cerr << "error deleting from user_files" << std::endl;
                return false;
            } else
                QSqlDatabase::database().commit();
            db.close();
            return true;
        } else {
            db.close();
            return false;
        }
    } else {
        //spdlog::error("error opened db");
        std::cerr << "error opened db" << std::endl;
        return false;
    }
}


_int Database::getIdFilenameDB(QString email_owner, QString filename) {
    if (db.open()) {
        //spdlog::debug("correct opened db to control password");
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        //take id of the filename thanks to the filename and to the email_owner
        qry.prepare("SELECT id FROM files WHERE email_owner = :email_owner and filename = :filename");
        qry.bindValue(":email_owner", email_owner);
        qry.bindValue(":filename", filename);
        if (!qry.exec()) {
            //spdlog::error("error while retrieve id from files table");
            std::cerr << "error while retrieve id from files table" << std::endl;
            return false;
        }

        qry.first();
        _int idFilename = qry.value(0).toInt();
        QSqlDatabase::database().commit();
        db.close();
        return idFilename;
    } else {
        //spdlog::error("error opened db");
        std::cerr << "error opened db" << std::endl;
        return 0;
    }
}


bool Database::updateLastAccessDB(QString email, _int idFilename) {

    if (db.open()) {
        //spdlog::debug("correct opened db to control password");
        QSqlDatabase::database().transaction();
        QSqlQuery qry;

        qry.prepare("UPDATE user_files SET last_access = :last_access WHERE email = :email and id = :idFilename");
        QDateTime dateTime = dateTime.currentDateTime();
        qry.bindValue(":last_access", dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
        qry.bindValue(":email", email);
        qry.bindValue(":idFilename", idFilename);
        if (!qry.exec()) {
            //spdlog::error("error while updating last_access in user_files");
            std::cerr << "error while updating last_access in user_files" << std::endl;
            return false;
        }
        QSqlDatabase::database().commit();
        db.close();
        return true;
    } else {
        //spdlog::error("error opened db");
        std::cerr << "error opened db" << std::endl;
        return false;
    }
}

bool Database::canOpenFileDB(const UserManagementMessage &userManagementMessage) {
    if (db.open()) {
        //spdlog::debug("correct opened db to control password");
        _int idFilename = getIdFilenameDB(userManagementMessage.getEmail_owner(), userManagementMessage.getFilename());
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT email FROM user_files WHERE id = :id AND email = :email");
        qry.bindValue(":id", idFilename);
        qry.bindValue(":email", userManagementMessage.getEmail_collaborator());
        if (!qry.exec()) {
            std::cerr << "error while select email in canOpenFileDB";
            return false;
        }
        qry.first();
        QString canOpenFile = qry.value(0).toString();
        QSqlDatabase::database().commit();
        db.close();
        if (QString::compare(canOpenFile, userManagementMessage.getEmail_collaborator()) == 0)
            return true;
        else
            return false;
    } else {
        //spdlog::error("error opened db");
        std::cerr << "error opened db" << std::endl;
        return false;
    }
}


