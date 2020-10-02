//
// Created by pinoOgni on 10/08/20.
//

#include "renameordelete.h"
#include "../../../cmake-build-debug/VirgiliumClient_autogen/include/ui_renameordelete.h"
#include "deletefile.h"
#include "renamefile.h"


renameOrDelete::renameOrDelete(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::renameOrDelete) {
    ui->setupUi(this);
}

renameOrDelete::~renameOrDelete() {
    delete ui;
}


void renameOrDelete::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            on_delete_2_clicked();
            break;
        default:
            QDialog::keyPressEvent(e);
    }
}

void renameOrDelete::receiveData_2(ClientStuff *client, QString email, QString filename, User user) {
    //bridge between secondlogin and delete or rename dialog
    this->currentUser = user;
    this->email = email;
    this->filename = filename;
    this->client = client;
    qDebug() << "receiveData_2 renameordelete" << email << filename;
}

void renameOrDelete::on_delete_2_clicked() {
    qDebug() << "on_delete_2_clicked";
    deleteFile *newDeleteFile = new deleteFile(this);
    connect(this, SIGNAL(sendData_2(ClientStuff * , QString, QString)), newDeleteFile,
            SLOT(receiveData_2(ClientStuff * , QString, QString)));
    emit sendData_2(client, email, filename);

    //close the qdialog deletefile, send a signal to secondwindow
    connect(newDeleteFile, &deleteFile::Want2Close3, this, &renameOrDelete::Want2Close2);

    newDeleteFile->show();
    this->close();
}

void renameOrDelete::on_rename_clicked() {
    qDebug() << "on_rename_clicked";
    renameFile *newRenameFile = new renameFile(this);
    connect(this, SIGNAL(sendData_2(ClientStuff * , QString, QString)), newRenameFile,
            SLOT(receiveData_2(ClientStuff * , QString, QString)));
    emit sendData_2(client, email, filename);


    //close the qdialog renamefile, send a signal to secondwindow
    connect(newRenameFile, &renameFile::Want2Close3, this, &renameOrDelete::Want2Close2);

    newRenameFile->show();
    this->close();

}

void renameOrDelete::on_cancel_clicked() {
    this->close();
}

void renameOrDelete::on_openTextEditor_clicked() {
    QString path = this->email + "/" + this->filename;
    textEditor = new TextEditor(nullptr, client->getSocket(), path, this->currentUser);
    textEditor->setAttribute(Qt::WA_DeleteOnClose);
    close();
    textEditor->show();
}