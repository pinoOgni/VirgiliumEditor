//
// Created by pinoOgni on 10/08/20.
//

#ifndef UNSUBSCRIBEFILE_H
#define UNSUBSCRIBEFILE_H

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

class unsubscribeFile : public QDialog
{
    Q_OBJECT

public:
    explicit unsubscribeFile(QWidget *parent = nullptr);
    ~unsubscribeFile();

private:
    Ui::unsubscribeFile *ui;

    QString email;
    QString filename;
    ClientStuff * client;
    QString email_owner;
    User currentUser;
    TextEditor *textEditor;

private slots:

    void keyPressEvent(QKeyEvent *) override;

    void receiveData_2(ClientStuff *, QString, QString, QString, User);

    void on_unsubscribe_clicked();

    void on_cancel_clicked();
    void isUnsubscribed(bool);

    void on_pushButton_clicked();

signals:
    void Want2Close2();
};

#endif // UNSUBSCRIBEFILE_H
