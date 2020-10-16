//
// Created by pinoOgni on 10/08/20.
//

#include "renameordelete.h"
#include "../../../cmake-build-debug/VirgiliumClient_autogen/include/ui_renameordelete.h"
#include "deletefile.h"
#include "renamefile.h"


renameOrDelete::renameOrDelete(QWidget *parent) : QDialog(parent), ui(new Ui::renameOrDelete) {
    ui->setupUi(this);

    this->setWindowTitle("Virgilium");
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/Icons/v.png"), QSize(), QIcon::Normal, QIcon::On);
    this->setWindowIcon(icon);
}

renameOrDelete::~renameOrDelete() {
    //spdlog::debug("rename or delete ~");
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
    //spdlog::debug("receiveData_2 renameordelete");
}

void renameOrDelete::on_delete_2_clicked() {
    deleteFile *newDeleteFile = new deleteFile(this);
    connect(this, SIGNAL(sendData_2(ClientStuff * , QString, QString)), newDeleteFile,
            SLOT(receiveData_2(ClientStuff * , QString, QString)));
    emit sendData_2(client, email, filename);

    //close the qdialog deletefile, send a signal to secondwindow
    connect(newDeleteFile, &deleteFile::Want2Close3, this, &renameOrDelete::slotWant2Close2);

    newDeleteFile->show();
    //this->close();
}

void renameOrDelete::on_rename_clicked() {
    //spdlog::debug("on_rename_clicked");
    auto *newRenameFile = new renameFile(this);
    //newRenameFile->setAttribute(Qt::WA_DeleteOnClose);
    connect(this, SIGNAL(sendData_2(ClientStuff * , QString, QString)), newRenameFile,
            SLOT(receiveData_2(ClientStuff * , QString, QString)));
    emit sendData_2(client, email, filename);

    //close the qdialog renamefile, send a signal to secondwindow
    connect(newRenameFile, &renameFile::Want2Close3, this, &renameOrDelete::slotWant2Close2);

    newRenameFile->show();
    //this->close();
}

void renameOrDelete::on_cancel_clicked() {
    this->close();
}

void renameOrDelete::slotWant2Close2() {
    emit Want2Close2();
    this->close();
}

void renameOrDelete::on_openTextEditor_clicked() {
    QString path = this->email + "/" + this->filename;
    textEditor = new TextEditor(nullptr, client->getSocket(), path, this->currentUser);
    textEditor->setAttribute(Qt::WA_DeleteOnClose);
    this->close();
    textEditor->show();
}
