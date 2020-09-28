//
// Created by pinoOgni on 10/08/20.
//

#include "managecollaborators.h"
#include "ui_managecollaborators.h"

manageCollaborators::manageCollaborators(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::manageCollaborators)
{
    ui->setupUi(this);
}

manageCollaborators::~manageCollaborators()
{
    delete ui;
}

void manageCollaborators::on_add_clicked()
{
    QString password = ui->add_password->text();
    QString email_collaborator = ui->add_collaborator->text();

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    UserManagementMessage userManagementMessage =
            UserManagementMessage(client->getSocket()->getClientID(),email_collaborator,
                                  email,
                                  filename,
                                  QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Sha224));

    client->getSocket()->send(ADD_COLLABORATOR,userManagementMessage);
    qDebug() << "add collaborator on push button clicked " << userManagementMessage.getEmail_owner();

    //every time the user push on "rename" I connect a signal
    connect(client, &ClientStuff::isCollaboratorAdded,this,&manageCollaborators::isCollaboratorAdded);
}

void manageCollaborators::isCollaboratorAdded(bool res) {
    if(res) {
        QMessageBox::information(this,"Done","Collaborator added");
        disconnect(client, &ClientStuff::isCollaboratorAdded,this,&manageCollaborators::isCollaboratorAdded);
        this->close();
    } else {
        QMessageBox::information(this,"Error","Errore while adding collaborator");

        //If there is an error, the signal is disconnected so only one message will be show to the user
        disconnect(client, &ClientStuff::isCollaboratorAdded,this,&manageCollaborators::isCollaboratorAdded);
    }
    ui->add_password->clear();
    ui->add_collaborator->clear();
    emit Want2Close2();
}

void manageCollaborators::on_remove_clicked()
{
    QString password = ui->remove_password->text();
    QString email_collaborator = ui->remove_collaborator->text();

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    UserManagementMessage userManagementMessage =
            UserManagementMessage(client->getSocket()->getClientID(),email_collaborator,
                                  email,
                                  filename,
                                  QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Sha224));

    client->getSocket()->send(REMOVE_COLLABORATOR,userManagementMessage);
    qDebug() << "remove collaborator on push button clicked " << userManagementMessage.getEmail_owner();

    //every time the user push on "rename" I connect a signal
    connect(client, &ClientStuff::isCollaboratorRemoved,this,&manageCollaborators::isCollaboratorRemoved);
}

void manageCollaborators::isCollaboratorRemoved(bool res) {
    if(res) {
        QMessageBox::information(this,"Done","Collaborator removed");
        this->close();
    } else {
        QMessageBox::information(this,"Error","Errore while removing collaborator");

        //If there is an error, the signal is disconnected so only one message will be show to the user
        disconnect(client, &ClientStuff::isCollaboratorRemoved,this,&manageCollaborators::isCollaboratorRemoved);
    }
    ui->remove_password->clear();
    ui->remove_collaborator->clear();
    emit Want2Close2();
}

void manageCollaborators::keyPressEvent(QKeyEvent *e) {
    switch (e->key ()) {
           case Qt::Key_Return:
           case Qt::Key_Enter:
             break;
           default:
               QDialog::keyPressEvent (e);
           }
}

void manageCollaborators::receiveData_2(ClientStuff *client,QString email,QString filename) {
    this->email = email;
    this->filename = filename;
    this->client = client;


    QString displayText = "You can add or remove collaborator to ' ";
    displayText.append(filename).append(" ' file!");
    ui->label->setText(displayText);
    qDebug() << "receiveData_2 managecollaborators" << email << filename;
}

void manageCollaborators::on_cancel_clicked()
{
    this->close();
}
