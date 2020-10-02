//
// Created by pinoOgni on 10/08/20.
//

#ifndef RENAMEORDELETE_H
#define RENAMEORDELETE_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include "../clientstuff.h"
#include "TextEditor.h"
#include <QCryptographicHash>


namespace Ui {
    class renameOrDelete;
}

class renameOrDelete : public QDialog {
Q_OBJECT

public:
    explicit renameOrDelete(QWidget *parent = nullptr);

    ~renameOrDelete() override;

private:
    Ui::renameOrDelete *ui;
    QString email;
    QString filename;
    ClientStuff *client;
    User currentUser;
    TextEditor *textEditor;
private slots:

    void keyPressEvent(QKeyEvent *) override;

    void receiveData_2(ClientStuff *, QString, QString, User);

    void on_delete_2_clicked();

    void on_rename_clicked();

    void on_cancel_clicked();

    void on_openTextEditor_clicked();

signals:
    void sendDataToEditor(ClientSocket *);

    //send client, email and password to deletefile dialog or to renamefile dialog
    void sendData_2(ClientStuff *, QString, QString);

    void Want2Close2();
};

#endif // RENAMEORDELETE_H
