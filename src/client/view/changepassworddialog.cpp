//
// Created by pinoOgni on 10/08/20.
//

#include "changepassworddialog.h"
#include "ui_changepassworddialog.h"

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePasswordDialog)
{
    ui->setupUi(this);
}

ChangePasswordDialog::~ChangePasswordDialog()
{
    delete ui;
}


void ChangePasswordDialog::on_ok_clicked()
{
    QString oldPsw = ui->old_password->text();
    QString newPsw = ui->new_password->text();

    if(oldPsw.isEmpty() || newPsw.isEmpty()) {
        QMessageBox::warning(this,"Error","Old or new password are empty");
    } else {
        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        ChangePasswordMessage changePasswordMessage =
                ChangePasswordMessage (client->getSocket()->getClientID(),email,
                                    QCryptographicHash::hash(oldPsw.toUtf8(),QCryptographicHash::Sha224),
                                    QCryptographicHash::hash(newPsw.toUtf8(),QCryptographicHash::Sha224));

        client->getSocket()->send(CHANGE_PASSWORD,changePasswordMessage);
        qDebug() << "change password push button clicked " << changePasswordMessage.getEmail();

    }
}


void ChangePasswordDialog::on_cancel_clicked()
{
    this->close();
}

void ChangePasswordDialog::receiveData(ClientStuff * client,QString email) {
    this->email = email;
    this->client = client;
    qDebug() << "receiveData changepassworddialog" << this->email;



    //every time the user push on "change" I connect a signal
    connect(client, &ClientStuff::isPswChanged,this,&ChangePasswordDialog::isPswChanged);
}

void ChangePasswordDialog::keyPressEvent(QKeyEvent *e) {
 switch (e->key ()) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            qDebug ("Return/enter pressed");
                on_ok_clicked();
        break;

        default:
            QDialog::keyPressEvent (e);
        }
}


void ChangePasswordDialog::isPswChanged(bool res) {
    qDebug() << "isPswChanged";
    if(res) {
        QMessageBox::information(this,"Done","Password changed");
        this->close();
        disconnect(client, &ClientStuff::isPswChanged,this,&ChangePasswordDialog::isPswChanged);
    } else {
        QMessageBox::warning(this,"Error","Old password is not correct!");
        disconnect(client, &ClientStuff::isPswChanged,this,&ChangePasswordDialog::isPswChanged);

        // called when already looking up or connecting/connected to "localhost"
    }
}