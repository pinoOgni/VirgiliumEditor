//
// Created by pinoOgni on 10/08/20.
//

#include <common/messages/InvitationMessage.h>
#include "PersonalPage.h"
#include "../../../cmake-build-debug/VirgiliumClient_autogen/include/ui_PersonalPage.h"
#include "changepassworddialog.h"
#include "renameordelete.h"
#include "managecollaborators.h"
#include "unsubscribefile.h"
#include <QCloseEvent>


void PersonalPage::closeEvent(QCloseEvent *event) {
    /*
     QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Virgilium Client",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
                                                                */

    //se facciamo così si chiude tutto il client
    User u = User(email);
    UserMessage userMessage = UserMessage(client->getSocket()->getClientID(), u);
    client->getSocket()->send(LOGOUT, userMessage);

    emit closeEditor();
    event->accept();
}


PersonalPage::PersonalPage(QWidget *parent) : QMainWindow(parent), ui(new Ui::PersonalPage) {
    ui->setupUi(this);

    QString theme = "background: #";
    theme.append("eff0f6");
    this->setStyleSheet(theme);

    this->setWindowTitle("Virgilium");
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/Icons/v.png"), QSize(), QIcon::Normal, QIcon::On);
    this->setWindowIcon(icon);

    //date and time
    QDateTime dateTime = dateTime.currentDateTime();
    QString dateTimeString = dateTime.toString("dd/MM/yyyy  hh:mm:ss");
    QFont serifFont("Times", 14, QFont::Bold);
    ui->label_orario->setFont(serifFont);
    ui->label_orario->setText(dateTimeString);

    timer = new QTimer(this);;
    connect(timer, SIGNAL(timeout()), this, SLOT(time_label()));
    timer->start(1000);

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateTimerSlot()));
    updateTimer->start(60000); //every minute the GUI is uptaded to look for modification of collaborators
}

PersonalPage::~PersonalPage() {
    //spdlog::debug( "~PersonalPage");
    disconnect(timer, SIGNAL(timeout()), this, SLOT(time_label()));
    disconnect(updateTimer, SIGNAL(timeout()), this, SLOT(updateTimerSlot()));
    delete ui;
}


void PersonalPage::updateTimerSlot() {
    connect(client, &ClientStuff::getAllData, this, &PersonalPage::getAllData2);
    getAllData();
}

void PersonalPage::time_label() {
    QDateTime dateTime = dateTime.currentDateTime();
    ui->label_orario->setText(dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
}

void PersonalPage::on_logout_clicked() {
    this->close();
    emit Want2Close();
}

void PersonalPage::receiveData(QString string, ClientStuff *client) {
    email = string;
    this->client = client;

    //spdlog::debug("receiveData");
    connect(client, &ClientStuff::getFilesOwner, this, &PersonalPage::getFilesOwner2);
    connect(client, &ClientStuff::getInfoUser, this, &PersonalPage::getInfoUser2);
    connect(client, &ClientStuff::getUserFiles, this, &PersonalPage::getUserFiles2);

    connect(client, &ClientStuff::getAllData, this, &PersonalPage::getAllData2);

    //every time the user push on "new file" I connect a signal
    connect(client, &ClientStuff::isFileCreated, this, &PersonalPage::isFileCreated);

    getAllData();
}

void PersonalPage::getAllData() {
    User u = User(email);
    UserMessage um = UserMessage(client->getSocket()->getClientID(), u);
    client->getSocket()->send(GET_ALL_DATA, um);
}

void PersonalPage::getAllData2(UserMessage &u, _int row1, std::vector<FilesMessage> &filesMessage1, _int row2,
                               std::vector<FilesMessage> &filesMessage2) {
    //spdlog::debug("PersonalPage::getAllData2");
    this->currentUser = u.getUser();
    getInfoUser2(u);
    getFilesOwner2(row1, filesMessage1);
    getUserFiles2(row2, filesMessage2);
    disconnect(client, &ClientStuff::getAllData, this, &PersonalPage::getAllData2);
}


void PersonalPage::getInfoUser() {
    User u = User(email);
    UserMessage um = UserMessage(client->getSocket()->getClientID(), u);
    client->getSocket()->send(GET_INFO_USER, um);
}

void PersonalPage::getInfoUser2(UserMessage &u) {
    //spdlog::debug("PersonalPage::getInfoUser2");
    QFont serifFont("Times", 14, QFont::Bold);
    ui->firstName->setText(u.getUser().getEmail());
    ui->firstName->setFont(serifFont);
    ui->lastName->setText(u.getUser().getFirstName());
    ui->lastName->setFont(serifFont);
    ui->user_email->setText(u.getUser().getLastName());
    ui->user_email->setFont(serifFont);

}

//retrieve files which are owned by the user
void PersonalPage::getFilesOwner() {
    User u = User(email);
    UserMessage um = UserMessage(client->getSocket()->getClientID(), u);
    client->getSocket()->send(GET_FILES_OWNER, um);
}

//receive data from mainwindow who receive data from clientstuff who recevice data from the server
void PersonalPage::getFilesOwner2(_int row, std::vector<FilesMessage> &filesMessage) {
    //spdlog::debug("PersonalPage::getFilesOwner2");
    ui->tableWidget->clearContents();

    ui->tableWidget->setRowCount(row);
    QTableWidgetItem *temp;
    int i = 0;
    for (auto item: filesMessage) {
        item.printUserInfo();
        temp = new QTableWidgetItem(item.getFilename());
        temp->setFlags(Qt::ItemIsEnabled);
        ui->tableWidget->setItem(i, 0, temp);
        temp = new QTableWidgetItem(item.getLast_access());
        temp->setFlags(Qt::ItemIsEnabled);
        ui->tableWidget->setItem(i, 1, temp);

        temp = new QTableWidgetItem(item.getCollaborators().join(" , "));
        temp->setFlags(Qt::ItemIsEnabled);
        ui->tableWidget->setItem(i, 2, temp);
        i++;
    }
}

void PersonalPage::getUserFiles() {
    User u = User(email);
    UserMessage um = UserMessage(client->getSocket()->getClientID(), u);
    client->getSocket()->send(GET_FILES_COLLABORATOR, um);
}

void PersonalPage::getUserFiles2(_int row, std::vector<FilesMessage> &filesMessage) {
    //spdlog::debug("PersonalPage::getUserFiles2");
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(row);
    QTableWidgetItem *temp;
    int i = 0;
    for (auto item: filesMessage) {
        item.printUserInfo();
        temp = new QTableWidgetItem(item.getFilename());
        temp->setFlags(Qt::ItemIsEnabled);
        ui->tableWidget_2->setItem(i, 0, temp);
        temp = new QTableWidgetItem(item.getOwner());
        temp->setFlags(Qt::ItemIsEnabled);
        ui->tableWidget_2->setItem(i, 1, temp);

        temp = new QTableWidgetItem(item.getCollaborators().join(" , "));
        temp->setFlags(Qt::ItemIsEnabled);
        ui->tableWidget_2->setItem(i, 2, temp);

        temp = new QTableWidgetItem(item.getLast_access());
        temp->setFlags(Qt::ItemIsEnabled);
        ui->tableWidget_2->setItem(i, 3, temp);

        i++;
    }
}

//first table: file created by the user
void PersonalPage::on_tableWidget_cellDoubleClicked(int row, int column) {
    //cell is different from item
    if (ui->tableWidget->item(row, column) == nullptr) {
        QMessageBox::warning(this, "Warning", "NULL");
    } else if (column == 0) {
        auto *newRenameOrDelete = new renameOrDelete(this);
        newRenameOrDelete->setAttribute(Qt::WA_DeleteOnClose);
        connect(this, SIGNAL(sendData_2(ClientStuff * , QString, QString, User)), newRenameOrDelete,
                SLOT(receiveData_2(ClientStuff * , QString, QString, User)));
        emit sendData_2(this->client, email, ui->tableWidget->item(row, column)->text(), this->currentUser);

        newRenameOrDelete->show();
        //spdlog::debug("RENAME OR DELETE FILE");

        //close renameordelete and refresh table files
        connect(newRenameOrDelete, SIGNAL(Want2Close2()), this, SLOT(getFilesOwner()));
    } else if (column == 1) {
        //TODO add something else?
    } else if (column == 2) {
        auto *manageC = new manageCollaborators(this);
        manageC->setAttribute(Qt::WA_DeleteOnClose);
        connect(this, SIGNAL(sendData_2(ClientStuff * , QString, QString, User)), manageC,
                SLOT(receiveData_2(ClientStuff * , QString, QString, User)));
        emit sendData_2(this->client, email, ui->tableWidget->item(row, 0)->text(), this->currentUser);

        manageC->show();
        //spdlog::debug("ADD OR REMOVE COLLABORATOR");

        connect(manageC, SIGNAL(Want2Close2()), this, SLOT(getFilesOwner()));
    }
}

void PersonalPage::on_actionChange_Password_triggered() {
    auto *newPsw = new ChangePasswordDialog(this);
    newPsw->setAttribute(Qt::WA_DeleteOnClose);
    connect(this, SIGNAL(sendData(ClientStuff * , QString)), newPsw, SLOT(receiveData(ClientStuff * , QString)));
    emit sendData(client, email);

    newPsw->show();
}

void PersonalPage::on_actionDefault_triggered() {
    QString theme = "background: #";
    theme.append("eff0f6");
    this->setStyleSheet(theme);
}

void PersonalPage::on_actionLight_triggered() {
    QString theme = "background: #";
    theme.append("fffefc");
    this->setStyleSheet(theme);
}

//creation of new file: insert into files, control if there is a equal filename
void PersonalPage::newFile(QString email, QString filename) {

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    FileManagementMessage fileManagementMessage =
            FileManagementMessage(client->getSocket()->getClientID(), email,
                                  filename,
                                  ""); //password is not used to create a new file. Change??
    client->getSocket()->send(NEW_FILE, fileManagementMessage);

    //spdlog::debug("PersonalPage::newFile ");
}

void PersonalPage::on_newFile_clicked() {
    QString filename = ui->filename->text();

    if (filename.isEmpty())
        QMessageBox::warning(this, "Error", "Filename is empty");
    else
        newFile(this->email, filename);
}


void PersonalPage::on_filename_returnPressed() {
    on_newFile_clicked();
}

void PersonalPage::isFileCreated(bool res) {
    if (res) {
        ui->filename->setText("");
        //update first table
        getFilesOwner();
    } else {
        QMessageBox::warning(this, "Error", "Error creating file");
        disconnect(client, &ClientStuff::isFileCreated, this, &PersonalPage::isFileCreated);
    }

}

void PersonalPage::isRequestToCollaboratedReceived(bool res) {
    ui->invitationCode->setText("");
    if (res) {
        QMessageBox::information(this, "Done", "Now you are a collaborator, check your files");
        disconnect(client, &ClientStuff::isRequestToCollaboratedReceived, this,
                   &PersonalPage::isRequestToCollaboratedReceived);
        getUserFiles();
    } else {
        QMessageBox::warning(this, "Error",
                             "Error while adding you as collaborator, maybe yout url is wrong or it is expired");
        disconnect(client, &ClientStuff::isRequestToCollaboratedReceived, this,
                   &PersonalPage::isRequestToCollaboratedReceived);
    }

}


void PersonalPage::on_tableWidget_2_cellDoubleClicked(int row, int column) {
    if (ui->tableWidget_2->item(row, column) == nullptr) {
        QMessageBox::warning(this, "Warning", "NULL");
    }
    //for every column
    auto *unsubscribe = new unsubscribeFile(this);
    unsubscribe->setAttribute(Qt::WA_DeleteOnClose);
    connect(this, SIGNAL(sendData_3(ClientStuff * , QString, QString, QString, User)), unsubscribe,
            SLOT(receiveData_2(ClientStuff * , QString, QString, QString, User)));
    emit sendData_3(this->client, email, ui->tableWidget_2->item(row, 0)->text(),
                    ui->tableWidget_2->item(row, 1)->text(), this->currentUser);

    unsubscribe->show();
    //spdlog::debug("REMOVE MYSELF FROM A FILE");

    connect(unsubscribe, SIGNAL(Want2Close2()), this, SLOT(getUserFiles()));
}

void PersonalPage::on_requestToCollaborate_clicked() {
    QString invitationCode = ui->invitationCode->text();

    if (invitationCode.isEmpty())
        QMessageBox::warning(this, "Error", "Invitation code is empty");
    else
        requestToCollaborate(this->email, invitationCode);
}


void PersonalPage::on_invitationCode_returnPressed() {
    on_requestToCollaborate_clicked();
}

void PersonalPage::requestToCollaborate(QString email, QString invitationCode) {
    connect(client, &ClientStuff::isRequestToCollaboratedReceived, this,
            &PersonalPage::isRequestToCollaboratedReceived);

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    InvitationMessage invitationMessage =
            InvitationMessage(client->getSocket()->getClientID(), email,
                              invitationCode);
    client->getSocket()->send(REQUEST_TO_COLLABORATE, invitationMessage);
}
