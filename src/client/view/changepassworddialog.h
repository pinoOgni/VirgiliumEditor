//
// Created by pinoOgni on 10/08/20.
//

#ifndef VIRGILIUM_CHANGEPASSWORDDIALOG_H
#define VIRGILIUM_CHANGEPASSWORDDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include <QCryptographicHash>
#include "../clientstuff.h"

namespace Ui {
    class ChangePasswordDialog;
}

class ChangePasswordDialog : public QDialog {
Q_OBJECT

public:
    explicit ChangePasswordDialog(QWidget *parent = nullptr);

    ~ChangePasswordDialog() override;

private slots:

    void on_ok_clicked();

    void on_cancel_clicked();

    void receiveData(ClientStuff *, QString);

    void keyPressEvent(QKeyEvent *) override;

    /*
     * this slot is triggered when the server send a response
     */
    void isPswChanged(bool);

private:
    Ui::ChangePasswordDialog *ui;
    QString email;
    ClientStuff *client;
};

#endif // VIRGILIUM_CHANGEPASSWORDDIALOG_H
