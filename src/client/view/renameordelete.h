//
// Created by pinoOgni on 10/08/20.
//

#ifndef VIRGILIUM_RENAMEORDELETE_H
#define VIRGILIUM_RENAMEORDELETE_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include "../clientstuff.h"
#include "TextEditor.h"
#include "PersonalPage.h"
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
    PersonalPage *personalPage;
private slots:

    /*
     * ovverride to managare the keypressevent
     */
    void keyPressEvent(QKeyEvent *) override;

    /*
     * receive data from personalPage
     */
    void receiveData_2(ClientStuff *, QString, QString, User);

    void on_delete_2_clicked();

    void on_rename_clicked();

    void on_cancel_clicked();

    void on_openTextEditor_clicked();

    /*
     * this slot is connected tiwh Want2Close3 from deletefile or renamefile
     */
    void slotWant2Close2();

    /*
     * this slot is connected with canOpenFile signal from clientStuff
     */
    void canOpenFile(bool res);

signals:

    void sendDataToEditor(ClientSocket *);

    //send client, email and password to deletefile dialog or to renamefile dialog
    void sendData_2(ClientStuff *, QString, QString);

    void Want2Close2();
};

#endif // VIRGILIUM_RENAMEORDELETE_H
