//
// Created by pinoOgni on 10/08/20.
//

#ifndef VIRGILIUM_MANAGECOLLABORATORS_H
#define VIRGILIUM_MANAGECOLLABORATORS_H

#include <QDialog>
#include "../clientstuff.h"
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include <QCryptographicHash>


namespace Ui {
    class manageCollaborators;
}

class manageCollaborators : public QDialog {
Q_OBJECT

public:
    explicit manageCollaborators(QWidget *parent = nullptr);

    ~manageCollaborators() override;

private:
    Ui::manageCollaborators *ui;

    QString email;
    QString filename;
    ClientStuff *client;
private slots:

    void keyPressEvent(QKeyEvent *) override;

    void receiveData_2(ClientStuff *, QString, QString, User);

    void on_add_clicked();

    void on_remove_clicked();

    /*
    * this slot is triggered when the server send a response
    */
    void isInviteCreated(InvitationMessage);

    /*
    * this slot is triggered when the server send a response
    */
    void isCollaboratorRemoved(bool);

    /*
    * this slot is triggered when the server send a response
    */
    void canRemoveCollaborator(bool);

    void on_cancel_clicked();

signals:
    /*
     * signal used to tell something to the personalPage
     */
    void Want2Close2();
};

#endif // VIRGILIUM_MANAGECOLLABORATORS_H
