//
// Created by pinoOgni on 10/08/20.
//

#include "deletefile.h"
#include "ui_deletefile.h"

deleteFile::deleteFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deleteFile)
{
    ui->setupUi(this);

}

deleteFile::~deleteFile()
{
    delete ui;
}



void deleteFile::receiveData_2(ClientStuff *client,QString email,QString filename) {
    //bridge between secondlogin and delete or rename dialog
    this->email = email;
    this->filename = filename;
    this->client = client;

    QString displayText = "Are you sure to delete ' ";
    displayText.append(filename).append(" ' file? No collaborator can access it anymore. ");
    ui->label->setText(displayText);
    qDebug() << "receiveData_2 deletefile" << email << filename;

    //every time the user push on "delete" I connect a signal
    connect(client, &ClientStuff::isFileDeleted,this,&deleteFile::isFileDeleted);
}


void deleteFile::keyPressEvent(QKeyEvent *e) {
    switch (e->key ()) {
           case Qt::Key_Return:
           case Qt::Key_Enter:
                on_pushButton_clicked();
            break;
           default:
               QDialog::keyPressEvent (e);
           }
}


void deleteFile::on_pushButton_clicked()
{
    QString password = ui->password->text();
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    FileManagementMessage fileManagementMessage =
            FileManagementMessage(client->getSocket()->getClientID(),email,
                                  filename,
                                  QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Sha224));

    client->getSocket()->send(DELETE_FILE,fileManagementMessage);
    qDebug() << "delete file push button clicked " << fileManagementMessage.getEmail();


}


void deleteFile::isFileDeleted(bool res) {

    if(res) {
        QMessageBox::information(this,"Deleted","File deleted");
        disconnect(client, &ClientStuff::isFileDeleted,this,&deleteFile::isFileDeleted);
        this->close();
        emit Want2Close3();
    } else {
        QMessageBox::information(this,"Deleted","Errore while deleteing file");
        disconnect(client, &ClientStuff::isFileDeleted,this,&deleteFile::isFileDeleted);
    }
}

