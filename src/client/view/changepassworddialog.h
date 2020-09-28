//
// Created by pinoOgni on 10/08/20.
//

#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include <QCryptographicHash>
#include "../clientstuff.h"

namespace Ui {
class ChangePasswordDialog;
}

class ChangePasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePasswordDialog(QWidget *parent = nullptr);
    ~ChangePasswordDialog() override;

private slots:

    void on_ok_clicked();
    void on_cancel_clicked();
    void receiveData(ClientStuff *,QString);
    void keyPressEvent(QKeyEvent *) override;
    void isPswChanged(bool);

private:
    Ui::ChangePasswordDialog *ui;
    QString email;
    bool correctPsw;
    ClientStuff * client;
};

#endif // CHANGEPASSWORDDIALOG_H
