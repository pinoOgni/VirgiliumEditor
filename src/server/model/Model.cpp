//
// Created by pinoOgni on 26/09/20.
//

#include "Model.h"

Model::Model():IDSeed(1) {

}

void Model::insertActiveUser(ClientSocket *socket, const User &user) {
    clientToUser.emplace(socket,user);

}

void Model::removeActiveUser(ClientSocket *socket) {
    if(clientToUser.find(socket) != clientToUser.end())
        clientToUser.erase(socket);
}

User Model::getActiveUser(ClientSocket *socket) {
    return clientToUser.find(socket)->second;

}

bool Model::loginUser(User user)  {
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

void Model::removeUserFromEditor(ClientSocket *socket) {
    auto toRemove = std::find_if(fileToClients.begin(), fileToClients.end(),
                                 [socket](auto &pair){
                                          return socket == pair.second;
                                          }
                                 );
    fileToClients.erase(toRemove);
}

void Model::insertUserIntoEditor(ServerDocument * serverDocument, ClientSocket * socket) {
    this->fileToClients.insert(
            std::pair<ServerDocument *, ClientSocket *>(serverDocument,socket)
            );
}