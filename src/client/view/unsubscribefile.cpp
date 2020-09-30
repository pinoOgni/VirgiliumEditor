//
// Created by pinoOgni on 10/08/20.
//

#include "unsubscribefile.h"
#include "ui_unsubscribefile.h"

unsubscribeFile::unsubscribeFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::unsubscribeFile)
{
    ui->setupUi(this);
}

unsubscribeFile::~unsubscribeFile()
{
    delete ui;
}

void unsubscribeFile::on_unsubscribe_clicked()
{
    QString password = ui->password->text();

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    UserManagementMessage userManagementMessage =
            UserManagementMessage(client->getSocket()->getClientID(),email,
                                  email_owner,
                                  filename,
                                  QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Sha224));

    client->getSocket()->send(UNSUBSCRIBE,userManagementMessage);
    qDebug() << "unsubscribe on push button clicked " << userManagementMessage.getEmail_collaborator();


}

void unsubscribeFile::on_cancel_clicked()
{
    this->close();
}

void unsubscribeFile::keyPressEvent(QKeyEvent *e) {
    switch (e->key ()) {
           case Qt::Key_Return:
           case Qt::Key_Enter:
                on_unsubscribe_clicked();
             break;
           default:
               QDialog::keyPressEvent (e);
           }
}

void unsubscribeFile::receiveData_2(ClientStuff *client,QString email,QString filename, QString email_owner) {
    this->email = email;
    this->filename = filename;
    this->client = client;
    this->email_owner = email_owner;


    QString displayText = "You are unsubscribing from ' ";
    displayText.append(filename).append(" ' file!");
    ui->label->setText(displayText);
    qDebug() << "receiveData_2 unsubscribeFile" << email << filename;


    connect(client, &ClientStuff::isUnsubscribed,this,&unsubscribeFile::isUnsubscribed);
}

void unsubscribeFile::isUnsubscribed(bool res) {
    if(res) {
        QMessageBox::information(this,"Done","Unsubscription from file completed");
        this->close();
    } else {
        QMessageBox::information(this,"Error","Unsubscription from file gone wrong");

        //If there is an error, the signal is disconnected so only one message will be show to the user
    }
    disconnect(client, &ClientStuff::isUnsubscribed,this,&unsubscribeFile::isUnsubscribed);
    ui->password->clear();
    emit Want2Close2();

}

void unsubscribeFile::on_pushButton_clicked()
{
    //SIMONE PARTIAMO DA QUAAA
    QString path = this->email_owner + "/" + this->filename;
    textEditor = new TextEditor(nullptr, this->client->getSocket(), path);
    close();
    textEditor->show();
}
