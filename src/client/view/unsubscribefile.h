//
// Created by pinoOgni on 10/08/20.
//

#ifndef VIRGILIUM_UNSUBSCRIBEFILE_H
#define VIRGILIUM_UNSUBSCRIBEFILE_H

#include <QDialog>
#include "../clientstuff.h"
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include <QCryptographicHash>
#include "TextEditor.h"

namespace Ui {
    class unsubscribeFile;
}

class unsubscribeFile : public QDialog {
Q_OBJECT

public:
    explicit unsubscribeFile(QWidget *parent = nullptr);

    ~unsubscribeFile();

private:
    Ui::unsubscribeFile *ui;

    QString email;
    QString filename;
    ClientStuff *client;
    QString email_owner;
    User currentUser;
    TextEditor *textEditor;
    PersonalPage *personalPage;

private slots:

    void on_unsubscribe_clicked();

    void on_cancel_clicked();

    void on_pushButton_clicked();

    /*
     * override toi manage keypressevent
     */
    void keyPressEvent(QKeyEvent *) override;

    /*
     * receive data form PersonalPage
     */
    void receiveData_2(ClientStuff *, QString, QString, QString, User);

    /*
     * this slot is connected with isUnsubscribed from ClientStuff
     */
    void isUnsubscribed(bool);

    /*
     * this slot is connected withcanOpenFile from ClientStuff
     */
    void canOpenFile(bool);


signals:

    /*
     * signal between unsubscribefile and PersonalPage
     */
    void Want2Close2();
};

#endif // VIRGILIUM_UNSUBSCRIBEFILE_H
