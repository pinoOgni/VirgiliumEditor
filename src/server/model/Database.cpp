//
// Created by pinoOgni on 26/09/20.
//

#include <QtSql/QSqlQuery>
#include <QtCore/QDateTime>
#include "Database.h"

//1 --> ricrea i db vergini
//0 --> usa il DB con i dati che ci sono
#define TEST 0

Database::Database() {
    if(!db.open())
        initDatabase();
}

void Database::closeConnectionDB() {
    db.close();
}

void Database::initDatabase() {
    //delete databaase file only for testing
    QString dbPath = QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).filePath("database.db");

    if(TEST==1) {
        QString doNotTouch = QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).filePath("doNotTouch.db");
        QFile file (dbPath);
        file.remove();
        file.open(QIODevice::WriteOnly | QIODevice::Text);

        //database
        QFile::copy(":/database.db", dbPath);
        file.copy(doNotTouch);

    }
    qDebug() << "init database";
    db = QSqlDatabase::addDatabase("QSQLITE"); //"SQLITE"
    db.setDatabaseName(dbPath);

    if(!db.open()) {
             qDebug() << "error first try to open DB";
    } else {
        db.close();
        if(TEST==1)
            createTablesForTests(dbPath,db);
    }

}




void Database::createTablesForTests(QString dbPath, QSqlDatabase db) {

    QFileInfo checkfile(dbPath);
    if(checkfile.isFile()) {
        if(db.open()) {
            QSqlQuery qry;
            qDebug() << "correct opened db";

            if(!db.tables().contains("users")) {
                QString create_users = "CREATE TABLE users ("
                        "email VARCHAR(50) PRIMARY KEY,"
                        "firstName VARCHAR(20),"
                        "lastName VARCHAR(20),"
                        "password VARCHAR(255))";
                if(!qry.exec(create_users)) {
                    qDebug() <<"error create table users";
                }
                else {
                    QSqlDatabase::database().transaction();
                    qry.prepare("INSERT INTO users (email,firstName,lastName,password) VALUES(?,?,?,?)");
                    qry.bindValue(0,"pino@pino.com");
                    qry.bindValue(1,"GIUSEPPE");
                    qry.bindValue(2,"OGNIBENE");
                    qry.bindValue(3,QCryptographicHash::hash("pino",QCryptographicHash::Sha224));
                    if(!qry.exec()) {
                        qDebug() << "what??";
                    }
                    qry.prepare("INSERT INTO users (email,firstName,lastName,password) VALUES(?,?,?,?)");
                    qry.bindValue(0,"ale@ale.com");
                    qry.bindValue(1,"ALESSANDRO");
                    qry.bindValue(2,"PAGANO");
                    qry.bindValue(3,QCryptographicHash::hash("ale",QCryptographicHash::Sha224));
                    if(!qry.exec()) {
                        qDebug() << "what??";
                    }
                    QSqlDatabase::database().commit();
                    qry.prepare("INSERT INTO users (email,firstName,lastName,password) VALUES(?,?,?,?)");
                    qry.bindValue(0,"simo@simo.com");
                    qry.bindValue(1,"SIMONE");
                    qry.bindValue(2,"D'AMILO");
                    qry.bindValue(3,QCryptographicHash::hash("simo",QCryptographicHash::Sha224));
                    if(!qry.exec()) {
                        qDebug() << "what??";
                    }
                    QSqlDatabase::database().commit();
                    qry.prepare("INSERT INTO users (email,firstName,lastName,password) VALUES(?,?,?,?)");
                    qry.bindValue(0,"ste@ste.com");
                    qry.bindValue(1,"STEFANO");
                    qry.bindValue(2,"SURACI");
                    qry.bindValue(3,QCryptographicHash::hash("ste",QCryptographicHash::Sha224));
                    if(!qry.exec()) {
                        qDebug() << "what??";
                    }
                    QSqlDatabase::database().commit();
                }



                QString create_files = "CREATE TABLE files ("
                        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                        "filename VARCHAR(50),"
                        "email_owner VARCHAR(50),"
                        "last_access DATETIME NOT NULL)";
                if(!qry.exec(create_files)) {
                    qDebug() <<"error create table files";
                }
                else {
                    QString create_user_files = "CREATE TABLE user_files ("
                            "id INT,"
                            "email VARCHAR(50),"
                            "last_access DATETIME NOT NULL,"
                            "PRIMARY KEY (id,email))";
                    if(!qry.exec(create_user_files)) {
                        qDebug() <<"error create table files";
                    }
                    else {
                        //insert item in files then in user_files, retrieve the last id inserted

                        QSqlDatabase::database().transaction();
                        qry.prepare("INSERT INTO files(filename, email_owner,last_access) VALUES(?,?,?)");
                        QString filename = "provafile";
                        qry.bindValue(0,filename);
                        qry.bindValue(1,"pino@pino.com");
                        QDateTime dateTime = dateTime.currentDateTime();
                        qry.bindValue(2,dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
                        if(!qry.exec()) {
                            qDebug() << "error insert in files";
                        }

                        //aggiungo ale@ale.com e ste@ste.com al mio file "provafile"
                        qry.prepare("SELECT LAST_INSERT_ROWID();");
                        if(!qry.exec()) {
                            qDebug() << "error retrieve last id inserted in files";
                        }
                        qry.first();
                        int id = qry.value(0).toInt();

                        qry.prepare("INSERT INTO user_files(id, email, last_access) VALUES(?,?,?)");
                        qry.bindValue(0,id);
                        qry.bindValue(1,"ale@ale.com");
                        dateTime = dateTime.currentDateTime();
                        qry.bindValue(2,dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
                        if(!qry.exec()) {
                            qDebug() << "error insert in files";
                        }

                        qry.prepare("INSERT INTO user_files(id, email, last_access) VALUES(?,?,?)");
                        qry.bindValue(0,id);
                        qry.bindValue(1,"ste@ste.com");
                        dateTime = dateTime.currentDateTime();
                        qry.bindValue(2,dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
                        if(!qry.exec()) {
                            qDebug() << "error insert in files";
                        }


                        //altra prova

                        qry.prepare("INSERT INTO files(filename, email_owner,last_access) VALUES(?,?,?)");
                        filename = "ciao";
                        qry.bindValue(0,filename);
                        qry.bindValue(1,"pino@pino.com");
                        dateTime = dateTime.currentDateTime();
                        qry.bindValue(2,dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
                        if(!qry.exec()) {
                            qDebug() << "error insert in files";
                        }

                        //aggiungo ale@ale.com e ste@ste.com al mio file "provafile"
                        qry.prepare("SELECT LAST_INSERT_ROWID();");
                        if(!qry.exec()) {
                            qDebug() << "error retrieve last id inserted in files";
                        }
                        qry.first();
                        id = qry.value(0).toInt();

                        qry.prepare("INSERT INTO user_files(id, email, last_access) VALUES(?,?,?)");
                        qry.bindValue(0,id);
                        qry.bindValue(1,"ale@ale.com");
                        dateTime = dateTime.currentDateTime();
                        qry.bindValue(2,dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
                        if(!qry.exec()) {
                            qDebug() << "error insert in files";
                        }

                        qry.prepare("INSERT INTO user_files(id, email, last_access) VALUES(?,?,?)");
                        qry.bindValue(0,id);
                        qry.bindValue(1,"simo@simo.com");
                        dateTime = dateTime.currentDateTime();
                        qry.bindValue(2,dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
                        if(!qry.exec()) {
                            qDebug() << "error insert in files";
                        }


                        QSqlDatabase::database().commit();
                    }
                }
            }
            db.close();
           //QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        }
    }
     else {
            qDebug() << "error opened db";
      }

}


void Database::printErrorAndExit(const std::string &error) {
  std::cout << error;
  exit(-1);
}


bool Database::loginDB(User user) {

    if(db.open()) {
        qDebug() << "correct opened db to control password";
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT password FROM users WHERE email = :email");
        qry.bindValue(":email", user.getEmail());
        qry.exec();
        qry.first();
        QString psw = qry.value(0).toString();
        qDebug() << "password " << psw;
        QSqlDatabase::database().commit();
        db.close();
        if(QString::compare(psw,QCryptographicHash::hash(user.getPassword().toUtf8(),QCryptographicHash::Sha224))==0) {
           return true;
        }
        else {
            return false;
        }
    }
    else {
           qDebug() << "error opened db";
           return false;
     }
}

bool Database::signinDB(User user) {

    if(db.open()) {
        qDebug() << "correct opened db to control user already in db";
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT email FROM users WHERE email = :email");
        qry.bindValue(":email", user.getEmail());
        qry.exec();
        qry.first();
        QString emailDB = qry.value(0).toString();
        qDebug() << "email in DB" << emailDB;
        QSqlDatabase::database().commit();

        if(emailDB.isNull()) {
            QSqlDatabase::database().transaction();
            qry.prepare("INSERT INTO users (email,firstName,lastName,password) VALUES(?,?,?,?)");
            qry.bindValue(0,user.getEmail());
            qry.bindValue(1,user.getFirstName());
            qry.bindValue(2,user.getLastName());
            qry.bindValue(3,QCryptographicHash::hash(user.getPassword().toUtf8(),QCryptographicHash::Sha224));
            if(!qry.exec()) {
                qDebug() << "what??";
            }
            QSqlDatabase::database().commit();
            db.close();
            return true;
        }
        else {
            db.close();
           return false;
        }
    }
    else {
           qDebug() << "error opened db";
           return false;
     }
}



User Database::getInfoUser(User user) {
    User userReturn;
    if(db.open()) {
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT email,firstName,lastName FROM users WHERE email = :email");
        qry.bindValue(":email", user.getEmail());
        qry.exec();
        qry.next();

        userReturn = User(user.getEmail(),"psw",qry.value(1).toString(),qry.value(2).toString());

        QSqlDatabase::database().commit();
        db.close();
        return userReturn;

    }
    else {
        qDebug() << "error opened db when try to get user's info";
        userReturn = User();
        return userReturn;
    }
}

std::vector<FilesMessage> Database::getFilesOwner(User user) {
    std::vector<FilesMessage> filesMessage;
    if(db.open()) {
        QSqlDatabase::database().transaction();
        QSqlQuery qry, qry1;


        qry.prepare("SELECT count(id) FROM files where email_owner = :email_owner");
        qry.bindValue(":email_owner", user.getEmail());
        qry.exec();
        qry.first();

        qry.prepare("SELECT filename,last_access,id FROM files WHERE email_owner = :email_owner");
        qry.bindValue(":email_owner", user.getEmail());
        qry.exec();

        int row = 0;

        for(qry.first(); qry.isValid(); qry.next(), ++row) {
            int id = qry.value(2).toInt();
            qry1.prepare("SELECT email FROM user_files WHERE id = :id");
            qry1.bindValue(":id",id);
            qry1.exec();
            QStringList collaborators;

            while (qry1.next()) {
                //collaborators.append(qry1.value(0).toString()).append(" ");
                collaborators << qry1.value(0).toString();
            }
            filesMessage.emplace_back(0,qry.value(0).toString(),qry.value(1).toString(),collaborators,user.getEmail());
            collaborators.clear();
        }
        QSqlDatabase::database().commit();
        db.close();
        return filesMessage;

    }
    else {
        qDebug() << "error opened db when try to get user's info";
        return filesMessage;
    }
}


std::vector<FilesMessage>  Database::getUserFiles(User user) {
    std::vector<FilesMessage> filesMessage;
    if(db.open()) {
        QSqlDatabase::database().transaction();
        QSqlQuery qry, qry1;


        //SISTEMARE GROUP BY COME ALL DATA
        qry.prepare("SELECT files.filename, files.email_owner, files.id,  user_files.last_access FROM files, user_files WHERE files.id IN (SELECT user_files.id FROM user_files WHERE user_files.email = :email ) GROUP BY files.id, files.filename, user_files.last_access, files.email_owner");
        qry.bindValue(":email", user.getEmail());
        if(!qry.exec()) {
            qDebug() << "error retrieve file where this user collaborates";
        }

        int row = 0;

        for(qry.first(); qry.isValid(); qry.next(), ++row) {

            int id = qry.value(2).toInt();

            qry1.prepare("SELECT email FROM user_files WHERE id = :id");
            qry1.bindValue(":id",id);
            qry1.exec();
            QStringList collaborators;

            while (qry1.next()) {
                //collaborators.append(qry1.value(0).toString()).append(" ");
                collaborators << qry1.value(0).toString();
            }
            filesMessage.emplace_back(0,qry.value(0).toString(),qry.value(3).toString(),collaborators,qry.value(1).toString());
            collaborators.clear();

        }
        QSqlDatabase::database().commit();
        db.close();
        return filesMessage;
    }
    else {
        qDebug() << "error opened db when try to get user's info";
        return filesMessage;
    }
}



bool Database::renameFileDB(FileManagementMessage fileManagementMessage) {
    if(db.open()) {
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT password FROM users WHERE email = :email");
        qry.bindValue(":email", fileManagementMessage.getEmail());
        qry.exec();
        qry.first();
        QString psw = qry.value(0).toString();
        qDebug() << "password " << psw;
        qDebug() << "getPassword " << fileManagementMessage.getPassword();
        QSqlDatabase::database().commit();
        if(QString::compare(psw,fileManagementMessage.getPassword())==0) {

            QSqlDatabase::database().transaction();

            qry.prepare("UPDATE files SET filename = :newFilename WHERE email_owner = :email_owner AND filename = :oldFilename");
            qry.bindValue(":email_owner", fileManagementMessage.getEmail());
            qry.bindValue(":oldFilename",fileManagementMessage.getOldfilename());
            qry.bindValue(":newFilename",fileManagementMessage.getFilename());

            if(!qry.exec()) {
                return false;
                qDebug() << "what?? " << fileManagementMessage.getEmail() << " " << fileManagementMessage.getOldfilename() << " " << fileManagementMessage.getFilename();
            }
            QSqlDatabase::database().commit();
            db.close();
        }
        else {
            db.close();
            return false;
        }
    }
    else {
        qDebug() << "error opened db";
        return false;
    }
    return true;

}


bool Database::deleteFileDB(FileManagementMessage fileManagementMessage) {
    if(db.open()) {
        qDebug() << "correct opened db to control password";
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT password FROM users WHERE email = :email");
        qry.bindValue(":email", fileManagementMessage.getEmail());
        qry.exec();
        qry.first();
        QString psw = qry.value(0).toString();
        qDebug() << "password " << psw;
        QSqlDatabase::database().commit();
        if(QString::compare(psw,fileManagementMessage.getPassword())==0) {
            QSqlDatabase::database().transaction();

            //C'È UNA QUERY MIGLIORE (forse)

            /*
             * select 'utente ale@ale.com rimuove file suca_1 di conseguenza i file viene cancellato fisicamente ed elimanato anche da user_files (rimuovo tutti i collaboratori)' AS '';


                DELETE FROM user_files
                WHERE user_files.id IN (select files.id
                                        from files
                                        where files.filename="suca_1"
                                        and files.email_owner="ale@ale.com");

                select '-----PRIMA DELETE---' AS '';


                DELETE FROM files
                WHERE files.email_owner ="ale@ale.com"
                and files.filename = "suca_1";
              */

            //take id to delete files in user_files table
            qry.prepare("SELECT id FROM files WHERE email_owner = :email_owner and filename = :filename");
            qry.bindValue(":email_owner", fileManagementMessage.getEmail());
            qry.bindValue(":filename",fileManagementMessage.getFilename());
            if(!qry.exec()) {
                qDebug() << "error while retrieve id from files table";
                return false;
            }

            qry.first();
            int idToDelete = qry.value(0).toInt();
            qDebug() << "idToDelete " << idToDelete;

            //delete rows in user_files: i.e. delete collaborators, ALL COLLABORATORS

            qry.prepare("DELETE FROM user_files WHERE user_files.id = :idToDelete");
            qry.bindValue(":idToDelete",idToDelete);
            if(!qry.exec()) {
                qDebug() << "error while deleting rows from user_files table";
                return false;
            }


            //delete file in files table
            //TODO: delete the file from the file system



            qry.prepare("DELETE FROM files  WHERE files.email_owner = :email AND files.filename = :filename");
            qry.bindValue(":email", fileManagementMessage.getEmail());
            qry.bindValue(":filename",fileManagementMessage.getFilename());
            if(!qry.exec()) {
                qDebug() << "error while deleting file " << fileManagementMessage.getFilename() << "from files table";
                return false;
            }
            QSqlDatabase::database().commit();
            db.close();

            //TODO PINO delete file in the file system --> PROVA
            QString filename = QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/storageVirgilium/").append(fileManagementMessage.getEmail())).filePath(fileManagementMessage.getFilename());
            QFile file (filename);
            file.remove();
        }
        else {
            db.close();
            return false;
        }
    }
    else {
        qDebug() << "error opened db";
        return false;
    }
    return true;
}




bool Database::newFileDB(FileManagementMessage fileManagementMessage) {
    //creation of new file: insert into files, control if there is a equal filename
    QString filenameTemp;
    if(db.open()) {
        QSqlQuery qry;
        qDebug() << "correct opened db";
        QSqlDatabase::database().transaction();
        bool go=true;
        qint8 n=1;
        filenameTemp = fileManagementMessage.getFilename();
        qDebug() << "filename " << fileManagementMessage.getFilename() << "  filenameTemp " << filenameTemp;
        while (go) {
            qry.prepare("SELECT filename FROM files WHERE email_owner = :email_owner AND filename = :filenameTemp");
            qry.bindValue(":email_owner", fileManagementMessage.getEmail());
            qry.bindValue(":filenameTemp", filenameTemp);
            if(!qry.exec()) {
                qDebug() << "error select filename";
            }
            qry.first();
            QString filenameDB = qry.value(0).toString();

            if(filenameDB.isNull()) {
                //interrupt while loop
                go = false;
                qry.prepare("INSERT INTO files(filename, email_owner,last_access) VALUES(?,?,?)");
                qry.bindValue(0,filenameTemp);
                qry.bindValue(1,fileManagementMessage.getEmail());
                QDateTime dateTime = dateTime.currentDateTime();
                qry.bindValue(2,dateTime.toString("dd/MM/yyyy  hh:mm:ss"));

                if(!qry.exec()) {
                    qDebug() << "error retrieve last id inserted in files";
                }

                /*
                qry.prepare("SELECT LAST_INSERT_ROWID();");
                if(!qry.exec()) {
                    qDebug() << "error retrieve last id inserted in files";
                }
                qry.first();
                int id = qry.value(0).toInt();

                qry.prepare("INSERT INTO user_files(id, email, last_access) VALUES(?,?,?)");
                qry.bindValue(0,id);
                qry.bindValue(1,email_owner);
                QDateTime dateTime = dateTime.currentDateTime();
                qry.bindValue(2,dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
                if(!qry.exec()) {
                    qDebug() << "error insert in files";
                }
                */
            }
            else {
                filenameTemp = fileManagementMessage.getFilename();
                filenameTemp = filenameTemp.append("_").append(QString::number(n));
                n++;
                qDebug() << "filename " << fileManagementMessage.getFilename() << "  filenameTemp " << filenameTemp;
            }
        }
        QSqlDatabase::database().commit();
        db.close();
         //TODO PINO create file in the file system --> PROVA
        QString filenamePath;
        if(!QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/storageVirgilium/").append(fileManagementMessage.getEmail())).exists())
            QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/storageVirgilium/")).mkdir(fileManagementMessage.getEmail());

        filenamePath = QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/storageVirgilium/").append(fileManagementMessage.getEmail())).filePath(filenameTemp);

        qDebug() << "new file db" << filenamePath;

        QFile file (filenamePath);
        file.open(QIODevice::WriteOnly | QIODevice::Text);

        return true;
    }
    else {
        qDebug() << "error opened db";
        return false;
    }
}



bool Database::changePasswordDB(ChangePasswordMessage changePasswordMessage) {
    if(db.open()) {
        qDebug() << "correct opened db to control password";
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT password FROM users WHERE email = :email");
        qry.bindValue(":email", changePasswordMessage.getEmail());
        qry.exec();
        qry.first();
        QString psw = qry.value(0).toString();
        qDebug() << "password " << psw;
        //QCryptographicHash::hash(oldPsw.toUtf8(),QCryptographicHash::Sha224)
        if(QString::compare(psw,changePasswordMessage.getOldPsw())==0) {
            qry.prepare("UPDATE users SET password = :newPsw WHERE email = :email");
            qry.bindValue(":email", changePasswordMessage.getEmail());
            qry.bindValue(":newPsw",changePasswordMessage.getNewPsw());
            if(!qry.exec()) {
                qDebug() << "what??";
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
        qDebug() << "error opened db";
        return false;
    }
}




bool Database::addCollaboratorDB(UserManagementMessage userManagementMessage) {
    if(db.open()) {
        qDebug() << "correct opened db to control password";
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT password FROM users WHERE email = :email");
        qry.bindValue(":email", userManagementMessage.getEmail_owner());
        qry.exec();
        qry.first();
        QString psw = qry.value(0).toString();
        qDebug() << "password " << psw;

        qry.prepare("SELECT email FROM users WHERE email = :collaborator");
        qry.bindValue(":collaborator", userManagementMessage.getEmail_collaborator());
        qry.exec();
        qry.first();
        QString resUser = qry.value(0).toString();
        qDebug() << "resCollaborator " << resUser;

        qry.prepare("SELECT id FROM files WHERE filename =:filename");
        qry.bindValue(":filename", userManagementMessage.getFilename());
        qry.exec();
        qry.first();
        int id = qry.value(0).toInt();
        qDebug() << "id filename " << id;


        qry.prepare("SELECT email FROM user_files WHERE id =:id AND email =:email");
        qry.bindValue(":id",id);
        qry.bindValue(":email",userManagementMessage.getEmail_collaborator());
        qry.exec();
        qry.first();
        QString isAlreadyCollaborator = qry.value(0).toString();
        QSqlDatabase::database().commit();


        qDebug() << "resUser " << resUser << "id " << id << "isAlreadyCollaborator" << isAlreadyCollaborator;
        //if password is correct, the user that could be added as collaborator is in user table,
        //the file is in the files table and the user that could be as collaborator is not alrdeady a collaborator
        if(QString::compare(psw,userManagementMessage.getPassword())==0 && resUser.isEmpty()==false && id!=0 && isAlreadyCollaborator.isEmpty()) {
            QSqlDatabase::database().transaction();

            qry.prepare("INSERT INTO user_files(id, email, last_access) VALUES(?,?,?)");
            qry.bindValue(0,id);
            qry.bindValue(1,userManagementMessage.getEmail_collaborator());
            qry.bindValue(2,"dd/MM/yyyy  hh:mm:ss");
            if(!qry.exec()) {
                qDebug() << "error insert in user_files";
                QSqlDatabase::database().commit();
                db.close();
                return false;
            } else {
                QSqlDatabase::database().commit();
                db.close();
                return true;
            }
        }
        else {
            db.close();
            return false;
        }
    }
    else {
        qDebug() << "error opened db";
        return false;
    }
}


bool Database::removeCollaboratorDB(UserManagementMessage userManagementMessage) {
    if(db.open()) {
        qDebug() << "correct opened db to control password";
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT password FROM users WHERE email = :email");
        qry.bindValue(":email", userManagementMessage.getEmail_owner());
        qry.exec();
        qry.first();
        QString psw = qry.value(0).toString();
        qDebug() << "password " << psw;

        qry.prepare("SELECT email FROM users WHERE email = :collaborator");
        qry.bindValue(":collaborator", userManagementMessage.getEmail_collaborator());
        qry.exec();
        qry.first();
        QString resUser = qry.value(0).toString();
        qDebug() << "resCollaborator " << resUser;

        qry.prepare("SELECT id FROM files WHERE email_owner = :email_owner and filename = :filename");
        qry.bindValue(":email_owner", userManagementMessage.getEmail_owner());
        qry.bindValue(":filename",userManagementMessage.getFilename());
        qry.exec();
        qry.first();
        int id = qry.value(0).toInt();
        qDebug() << "id filename " << id;


        qry.prepare("SELECT email FROM user_files WHERE id =:id AND email =:email");
        qry.bindValue(":id",id);
        qry.bindValue(":email",userManagementMessage.getEmail_collaborator());
        qry.exec();
        qry.first();
        QString isAlreadyCollaborator = qry.value(0).toString();
        QSqlDatabase::database().commit();


        qDebug() << "resUser " << resUser << "id " << id << "isAlreadyCollaborator" << isAlreadyCollaborator;
        //if password is correct, the user that could be removed as collaborator is in user table,
        //the file is in the files table and the user that could be removed as collaborator is a collaborator
        if(QString::compare(psw,userManagementMessage.getPassword())==0 && resUser.isEmpty()==false && id!=0 && isAlreadyCollaborator.isEmpty()==false) {
            QSqlDatabase::database().transaction();

            qry.prepare("DELETE FROM user_files WHERE user_files.id = :idToDelete AND user_files.email = :email");
            qry.bindValue(":idToDelete",id);
            qry.bindValue(":email",userManagementMessage.getEmail_collaborator());
            if(!qry.exec()) {
                qDebug() << "error deleteting from user_files";
                QSqlDatabase::database().commit();
                db.close();
                return false;
            } else {
                qDebug() << "collaborator " << userManagementMessage.getEmail_collaborator() << " removed \n";
                QSqlDatabase::database().commit();
                db.close();
                return true;
            }
        }
        else {
            db.close();
            return false;
        }
    }
    else {
        qDebug() << "error opened db";
        return false;
    }
}

bool Database::unsubscribeDB(UserManagementMessage userManagementMessage) {
    if(db.open()) {
        qDebug() << "correct opened db to control password";
        QSqlDatabase::database().transaction();
        QSqlQuery qry;
        qry.prepare("SELECT password FROM users WHERE email = :email");
        qry.bindValue(":email", userManagementMessage.getEmail_collaborator());
        qry.exec();
        qry.first();
        QString psw = qry.value(0).toString();
        qDebug() << "password " << psw;

        qry.prepare("SELECT id FROM files WHERE filename =:filename AND email_owner =:email_owner");
        qry.bindValue(":filename", userManagementMessage.getFilename());
        qry.bindValue(":email_owner", userManagementMessage.getEmail_owner());
        qry.exec();
        qry.first();
        int id = qry.value(0).toInt();
        qDebug() << "id filename " << id;

        qry.prepare("SELECT email FROM user_files WHERE id =:id AND email =:email");
        qry.bindValue(":id",id);
        qry.bindValue(":email",userManagementMessage.getEmail_collaborator());
        qry.exec();
        qry.first();
        QString isCollaborator = qry.value(0).toString();
        QSqlDatabase::database().commit();


        qDebug() << "id " << id << "isCollaborator" << isCollaborator;

        if(QString::compare(psw,userManagementMessage.getPassword())==0 && id!=0 && isCollaborator.isEmpty()==false) {
            QSqlDatabase::database().transaction();
            qry.prepare("DELETE FROM user_files WHERE user_files.id = :idToDelete AND user_files.email = :email");
            qry.bindValue(":idToDelete",id);
            qry.bindValue(":email",userManagementMessage.getEmail_collaborator());
            if(!qry.exec()) {
                qDebug() << "error deleting from user_files";
                return false;
            } else
            QSqlDatabase::database().commit();
            db.close();
            return true;
        }
        else {
            db.close();
            return false;
        }
    }
    else {
        qDebug() << "error opened db";
        return false;
    }
}