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


class Model {

    std::multimap< ServerDocument * , ClientSocket*> fileToClients;
    //QMutex fileToClientsMutex;
    std::map<ClientSocket *, User> clientToUser;
    std::map<QString, QList<User>> activeClientsForDocument;
    std::atomic<quint32> IDSeed;

public:
    Model();

  void insertActiveUser(ClientSocket *socket, const User &user);
  void removeActiveUser(ClientSocket *socket);
  User getActiveUser(ClientSocket *socket);
  void removeUserFromEditor(ClientSocket *socket);
  void insertUserIntoEditor(ServerDocument *, ClientSocket *);
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

    QList<User> addActiveUser(const User &user, const QString &fileName);

    void removeActiveUser(const User &user, const QString &fileName);

};

#endif // VIRGILIUM_MODEL_H

