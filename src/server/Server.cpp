//
// Created by alex on 10/08/20.
//

#include <common/ClientSocket.h>
#include <common/constants.h>
#include <common/messages/CrdtMessage.h>
#include "Server.h"

void Server::incomingConnection(qintptr handle) {
    auto nuovoSocket =  new ClientSocket(this);
    if(!nuovoSocket->setSocketDescriptor(handle)){
        nuovoSocket->deleteLater();
        return;
    }
    nuovoSocket->setClientID(handle);

    connect(
            nuovoSocket,
            &QTcpSocket::stateChanged,
            this,
            &Server::onSocketStateChanged
            );
    connect(
            nuovoSocket,
            &ClientSocket::basicMessageReceived,
            this,
            &Server::onProcessBasicMessage
    );
    connect(
            nuovoSocket,
            &ClientSocket::userMessageReceived,
            this,
            &Server::onProcessUserMessage
    );
    connect(
            nuovoSocket,
            &ClientSocket::fileManagementMessageReceived,
            this,
            &Server::onFileManagementMessageReceived
    );
    connect(
            nuovoSocket,
            &ClientSocket::changePasswordMessageReceived,
            this,
            &Server::onChangePasswordMessageReceived
    );
    connect(
            nuovoSocket,
            &ClientSocket::userManagementMessageReceived,
            this,
            &Server::onUserManagementMessageReceived
    );


    //adesso dire al nuovo client che si è connesso
    BasicMessage basicMessage(handle);
    qDebug()<<"Sto mandando: " << handle << "\n";

    nuovoSocket->send(CLIENT_CONNECTED,basicMessage);

    /*    QByteArray blocco;
    QDataStream out(&blocco,QIODevice::ReadWrite);
    out.setVersion(Q_DATA_STREAM_VERSION);
    _int code = CLIENT_CONNECTED;
    out<<code;
    out<<basicMessage;
    nuovoSocket->write(blocco);*/
    qDebug()<<"ho mandato\n";
}

void Server::onMessageReceived(QByteArray data) {
    auto sender = dynamic_cast<ClientSocket *> (QObject::sender());

    auto id = sender->getClientID();
    qDebug()<<"qui arrivo popo";
    //_int code =
    this->processMessage(sender,data);

}

Server::Server(unsigned short port, Model& model):model(model) {
    if(!listen(QHostAddress::LocalHost, port)){
        qDebug()<<"Errore server";
        exit(-1);
    }
    qDebug()<<"Listening on address:" << this->serverAddress().toString()<<":" << this->serverPort() <<"\n";
    //connect(this,&Server::processBasicMessage,this,&Server::onProcessBasicMessage);
    connect(this,&Server::processCrdtMessage,this,&Server::onProcessCrdtMessage);
    connect(this,&Server::processUserMessage,this,&Server::onProcessUserMessage);

    //ale fai occhio
    //connect(this, &Server::processFilesMessage, this, &Server::onProcessFileMessage);


    //TODO PROVA PINO
    if(!QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/storageVirgilium/")).exists())
        QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).mkdir("storageVirgilium");
}

void Server::onSocketStateChanged(QTcpSocket::SocketState state) {

}

void Server::processMessage(ClientSocket *sender,QByteArray data) {
    QDataStream stream(&data,QIODevice::ReadOnly);
    _int code;
    stream >> code;

    switch(code){
        case BASIC_MESSAGE:{
            BasicMessage basicMessage;
            stream >>basicMessage;
            emit processBasicMessage(code,basicMessage);

        }
            break;
        case CRDT_MESSAGE: {
            CrdtMessage crdtMessage;
            stream >> crdtMessage;
            emit processCrdtMessage(code,crdtMessage);
        }
            break;

        case LOGIN:
        case SIGNUP:
        case GET_FILES_OWNER:
        case GET_INFO_USER:
        case GET_FILES_COLLABORATOR:
        case GET_ALL_DATA:
            {
           UserMessage userMessage = UserMessage();
           stream >> userMessage;
           emit processUserMessage(code,userMessage);
        }

        break;
        case RENAME_FILE:
        case DELETE_FILE:
        case NEW_FILE:

        {
            /*qua usare nuova classe message
             *
             * */
        }
        case CHANGE_PASSWORD:
            {
            /* forse avrà bisogno di gestione particolare
             * */
            }
        case ADD_COLLABORATOR:
        case REMOVE_COLLABORATOR:
        case UNSUBSCRIBE:
        {
            /*
             *
             * */

        }
    }

    }



void Server::onProcessBasicMessage(_int code, BasicMessage basicMessage) {
    //robe super basic
    switch(code){
        case CLIENT_CONNECTED:
            break;
    }

}
void Server::onProcessCrdtMessage(_int code,CrdtMessage crdtMessage) {
        //logica crdt
}

void  Server::dispatch() {

}

void Server::onProcessUserMessage(_int code,UserMessage userMessage) {
   // qDebug() << userMessage.getUser().printMessage();
   auto sender = dynamic_cast<ClientSocket*>(QObject::sender());
    qDebug() << "userMessage " << userMessage.getUser().printMessage();
    switch (code) {
        case LOGIN: {
            if (Model::loginUser(userMessage.getUser())) {
                sender->send(LOGIN_OK);
                qDebug() << "readClient L true";
            } else {
                sender->send(LOGIN_KO);
                qDebug() << "readClient L false";
            }
        }
        break;
        case SIGNUP: {
            if (Model::signinUser(userMessage.getUser())) {
                sender->send(SIGNUP_OK);
                qDebug() << "readClient S true";
            } else {
                sender->send(SIGNUP_KO);
                qDebug() << "readClient S false";
            }
        }
        break;
        case GET_INFO_USER: {
            User userReturn = model.getInfoUser(userMessage.getUser());
            // if(userReturn.getEmail().isEmpty())
            UserMessage userMessageReturn = UserMessage(sender->getClientID(),userReturn);
            sender->send(GET_INFO_USER,userMessageReturn);
        }
        break;
        case GET_FILES_OWNER: {
            std::vector<FilesMessage> filesMessage = model.getFilesOwner(userMessage.getUser());
            if(filesMessage.size()==0) {
                qDebug() << "get_files_owner ko";
                sender->send(GET_FILES_OWNER_KO, filesMessage);
            }
            else {
                qDebug() << "get_files_owner ok";

                //problema con sender dove lo metto?
                for (int i = 0; i < filesMessage.size(); i++)
                    filesMessage[i].setSender(sender->getClientID());
                sender->send(GET_FILES_OWNER_OK, filesMessage);
            }
        }
        break;
        case GET_FILES_COLLABORATOR: {
            std::vector<FilesMessage> filesMessage = model.getUserFiles(userMessage.getUser());
            if(filesMessage.size()==0) {
                qDebug() << "get_user_files ko";
                sender->send(GET_FILES_COLLABORATOR_OK, filesMessage);
            }
            else {
                qDebug() << "get_user_files ok";

                //problema con sender dove lo metto?
                for (int i = 0; i < filesMessage.size(); i++)
                    filesMessage[i].setSender(sender->getClientID());
                sender->send(GET_FILES_COLLABORATOR_KO, filesMessage);
            }
        }
        break;
        case GET_ALL_DATA: {
            User userReturn = model.getInfoUser(userMessage.getUser());
            // if(userReturn.getEmail().isEmpty())
            UserMessage userMessageReturn = UserMessage(sender->getClientID(),userReturn);

            std::vector<FilesMessage> filesOwner = model.getFilesOwner(userMessage.getUser());
            for (int i = 0; i < filesOwner.size(); i++)
                filesOwner[i].setSender(sender->getClientID());

            std::vector<FilesMessage> filesCollabs = model.getUserFiles(userMessage.getUser());
            for (int i = 0; i < filesCollabs.size(); i++)
                filesCollabs[i].setSender(sender->getClientID());

            sender->send(GET_ALL_DATA_OK, userMessageReturn,filesOwner,filesCollabs);
            qDebug() << "GET_ALL_DATA_OK ";
        }
        break;
    }
}


void Server::onFileManagementMessageReceived(_int code, FileManagementMessage fileManagementMessage) {
    auto sender = dynamic_cast<ClientSocket*>(QObject::sender());
    qDebug() << "onFileManagementMessageReceived ";
    switch(code) {
        case RENAME_FILE: {
            qDebug() << "rename_file: " << fileManagementMessage.getEmail() << "\n";
            if(model.renameFile(fileManagementMessage))
                sender->send(RENAME_FILE_OK);
            else
                sender->send(RENAME_FILE_KO);
        }
        break;
        case DELETE_FILE: {
            qDebug() << "delete_file: " << fileManagementMessage.getEmail() << "\n";
            if(model.deleteFile(fileManagementMessage))
                sender->send(DELETE_FILE_OK);
            else
                sender->send(DELETE_FILE_KO);
        }
        break;
        case NEW_FILE: {
            qDebug() << "new_file: " << fileManagementMessage.getEmail() << "\n";
            if(model.newFile(fileManagementMessage))
                sender->send(NEW_FILE_OK);
            else
                sender->send(NEW_FILE_KO);
        }
        break;
    }
}


void Server::onChangePasswordMessageReceived(_int code, ChangePasswordMessage changePasswordMessage) {
    auto sender = dynamic_cast<ClientSocket*>(QObject::sender());
    switch(code) {
        case CHANGE_PASSWORD: {
            qDebug() << "change_password: " << changePasswordMessage.getEmail() << "\n";
            if(model.changePassword(changePasswordMessage))
                sender->send(CHANGE_PASSWORD_OK);
            else
                sender->send(CHANGE_PASSWORD_KO);
        }
        break;
    }
}

void Server::onUserManagementMessageReceived(_int code, UserManagementMessage userManagementMessage) {
    auto sender = dynamic_cast<ClientSocket*>(QObject::sender());
    switch(code) {
        case ADD_COLLABORATOR: {
            qDebug() << "add_collaborator: " << userManagementMessage.getEmail_owner() << "\n";
            if(model.addCollaborator(userManagementMessage))
                sender->send(ADD_COLLABORATOR_OK);
            else
                sender->send(ADD_COLLABORATOR_KO);
        }
        break;
        case REMOVE_COLLABORATOR: {
            qDebug() << "remove_collaborator: " << userManagementMessage.getEmail_owner() << "\n";
            if(model.removeCollaborator(userManagementMessage))
                sender->send(REMOVE_COLLABORATOR_OK);
            else
                sender->send(REMOVE_COLLABORATOR_KO);
        }
        break;
        case UNSUBSCRIBE: {
            qDebug() << "unsubscribe: " << userManagementMessage.getEmail_collaborator() << "\n";
            if(model.unsubscribe(userManagementMessage))
                sender->send(UNSUBSCRIBE_OK);
            else
                sender->send(UNSUBSCRIBE_KO);
        }
        break;
    }
}