//
// Created by pinoOgni on 10/08/20.
//

#include "managecollaborators.h"
#include "../../../cmake-build-debug/VirgiliumClient_autogen/include/ui_managecollaborators.h"

manageCollaborators::manageCollaborators(QWidget *parent) : QDialog(parent), ui(new Ui::manageCollaborators) {
    ui->setupUi(this);

    this->setWindowTitle("Virgilium");
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/Icons/v.png"), QSize(), QIcon::Normal, QIcon::On);
    this->setWindowIcon(icon);
}

manageCollaborators::~manageCollaborators() {
    disconnect(client, &ClientStuff::isInviteCreated, this, &manageCollaborators::isInviteCreated);
    delete ui;
}

void manageCollaborators::on_add_clicked() {
    QString password = ui->add_password->text();
    QString email_collaborator = ui->add_collaborator->text();

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    UserManagementMessage userManagementMessage =
            UserManagementMessage(client->getSocket()->getClientID(), email_collaborator,
                                  email,
                                  filename,
                                  QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha224));

    client->getSocket()->send(CREATE_INVITE, userManagementMessage);

    //every time the user push on "rename" I connect a signal
    connect(client, &ClientStuff::isInviteCreated, this, &manageCollaborators::isInviteCreated);
}

void manageCollaborators::isInviteCreated(InvitationMessage invitationMessage) {
    if (!invitationMessage.getInvitationCode().isEmpty()) {
        auto *qd = new QDialog;
        qd->setWindowTitle("Invite created: it expires in 60 minutes");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icons/v.png"), QSize(), QIcon::Normal, QIcon::On);
        qd->setWindowIcon(icon);
        auto *vl = new QVBoxLayout;
        qd->setLayout(vl);

        auto label = new QLabel(invitationMessage.getInvitationCode());
        label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        auto *ok = new QPushButton("Ok");
        ok->setMaximumWidth(100);

        vl->addWidget(label);
        vl->addWidget(ok);

        QObject::connect(ok, &QPushButton::clicked,
                         [qd]() {
                             qd->close();
                         });

        qd->show();

        this->close();
    } else {
        QMessageBox::information(this, "Error", "Error while creating invite");

        //If there is an error, the signal is disconnected so only one message will be show to the user
        disconnect(client, &ClientStuff::isInviteCreated, this, &manageCollaborators::isInviteCreated);
    }
    ui->add_password->clear();
    ui->add_collaborator->clear();
    emit Want2Close2();
}

void manageCollaborators::on_remove_clicked() {
    QString password = ui->remove_password->text();
    QString email_collaborator = ui->remove_collaborator->text();

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    UserManagementMessage userManagementMessage =
            UserManagementMessage(client->getSocket()->getClientID(), email_collaborator,
                                  email,
                                  filename,
                                  QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha224));

    client->getSocket()->send(REMOVE_COLLABORATOR, userManagementMessage);

    //every time the user push on "rename" I connect a signal
    connect(client, &ClientStuff::isCollaboratorRemoved, this, &manageCollaborators::isCollaboratorRemoved);
    connect(client, &ClientStuff::canRemoveCollaborator, this, &manageCollaborators::canRemoveCollaborator);

}

void manageCollaborators::isCollaboratorRemoved(bool res) {
    if (res) {
        QMessageBox::information(this, "Done", "Collaborator removed");
        this->close();
    } else {
        QMessageBox::information(this, "Error", "Error while removing collaborator");

        //If there is an error, the signal is disconnected so only one message will be show to the user
        disconnect(client, &ClientStuff::isCollaboratorRemoved, this, &manageCollaborators::isCollaboratorRemoved);
        disconnect(client, &ClientStuff::canRemoveCollaborator, this, &manageCollaborators::canRemoveCollaborator);

    }
    ui->remove_password->clear();
    ui->remove_collaborator->clear();
    emit Want2Close2();
}

void manageCollaborators::canRemoveCollaborator(bool res) {
    if (!res) {
        QMessageBox::warning(this, "Error", "Collaborator " + ui->remove_collaborator->text() + " has the text editor open, retry later");
        //If there is an error, the signal is disconnected so only one message will be show to the user
        disconnect(client, &ClientStuff::isCollaboratorRemoved, this, &manageCollaborators::isCollaboratorRemoved);
        disconnect(client, &ClientStuff::canRemoveCollaborator, this, &manageCollaborators::canRemoveCollaborator);
        this->close();
    }
}


void manageCollaborators::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            break;
        default:
            QDialog::keyPressEvent(e);
    }
}

void manageCollaborators::receiveData_2(ClientStuff *client, QString email, QString filename, User user) {
    this->email = email;
    this->filename = filename;
    this->client = client;


    QString displayText = "You can add or remove collaborator to ' ";
    displayText.append(filename).append(" ' file!");
    ui->label->setText(displayText);
    //spdlog::debug("receiveData_2 managecollaborators");
}

void manageCollaborators::on_cancel_clicked() {
    this->close();
}
