//
// Created by pinoOgni on 10/08/20.
//

#include "renamefile.h"
#include "ui_renamefile.h"

renameFile::renameFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::renameFile)
{
    ui->setupUi(this);
}

renameFile::~renameFile()
{
    delete ui;
}

void renameFile::on_pushButton_clicked()
{
    QString password = ui->password->text();
    QString newFilename = ui->newFilename->text();

    //every time the user push on "rename" I connect a signal
    connect(client, &ClientStuff::isFileRenamed,this,&renameFile::isFileRenamed);

    if(QString::compare(oldFilename,newFilename)==0) {
        QMessageBox::warning(this,"Error","Old filename is equal to new filename");
    } else {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    FileManagementMessage fileManagementMessage =
            FileManagementMessage(client->getSocket()->getClientID(),email,
                                  newFilename,
                                  oldFilename,
                                  QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Sha224));

    client->getSocket()->send(RENAME_FILE,fileManagementMessage);
        spdlog::debug("rename file push button clicked ");
    }
}


void renameFile::keyPressEvent(QKeyEvent *e) {
    switch (e->key ()) {
           case Qt::Key_Return:
           case Qt::Key_Enter:
                on_pushButton_clicked();
            break;
           default:
               QDialog::keyPressEvent (e);
           }
}


void renameFile::receiveData_2(ClientStuff *client,QString email,QString oldFilename) {
    //bridge between secondlogin and delete or rename dialog
    this->email = email;
    this->oldFilename = oldFilename;
    this->client = client;


    QString displayText = "You are renaming the ' ";
    displayText.append(oldFilename).append(" ' file!");
    ui->label->setText(displayText);
    spdlog::debug("receiveData_2 renamefile email");
}

void renameFile::isFileRenamed(bool res) {
    if(res) {
        QMessageBox::information(this,"Done","Name changed");
        this->close();
        disconnect(client, &ClientStuff::isFileRenamed,this,&renameFile::isFileRenamed);
        emit Want2Close3();
    } else {
        QMessageBox::information(this,"Error","Errore while renaming file");

        //If there is an error, the signal is disconnected so only one message will be show to the user
        disconnect(client, &ClientStuff::isFileRenamed,this,&renameFile::isFileRenamed);
    }
}












