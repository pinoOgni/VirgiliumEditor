//
// Created by pinoOgni on 10/08/20.
//

#include <common/messages/InvitationMessage.h>
#include "PersonalPage.h"
#include "ui_PersonalPage.h"
#include "changepassworddialog.h"
#include "renameordelete.h"
#include "managecollaborators.h"
#include "unsubscribefile.h"


PersonalPage::PersonalPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PersonalPage)
{
    ui->setupUi(this);

    QString theme = "background: #";
    theme.append("eff0f6");
    this->setStyleSheet(theme);
    qDebug() << "theme " << theme;

        //date and time
        QDateTime dateTime = dateTime.currentDateTime();
        QString dateTimeString = dateTime.toString("dd/MM/yyyy  hh:mm:ss");
        QFont serifFont("Times", 14, QFont::Bold);
        ui->label_orario->setFont(serifFont);
        ui->label_orario->setText(dateTimeString);

        //connect to default connection to the DB
        //db = QSqlDatabase::database();

        //database
        //QString dbPath = QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).filePath("database.db");
        //databaseManagement(dbPath);

        timer = new QTimer(this);;
        connect(timer, SIGNAL(timeout()), this, SLOT(time_label()));
        timer->start(1000);

        //updateTimer = new QTimer(this);
        //connect(updateTimer, SIGNAL(timeout()), this, SLOT(getAllData()));
        //updateTimer->start(10000);


}


void PersonalPage::time_label() {
    //qDebug() << "time_label ";
    QDateTime dateTime = dateTime.currentDateTime();
    ui->label_orario->setText(dateTime.toString("dd/MM/yyyy  hh:mm:ss"));
}


PersonalPage::~PersonalPage()
{
    delete ui;
}

void PersonalPage::on_logout_clicked()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(time_label()));
    client->getSocket()->send(LOGOUT);
    this->close();
    emit Want2Close();
}


void PersonalPage::receiveData(QString string,ClientStuff* client) {
    email = string;
    this->client = client;

    qDebug() << "receiveData \n";
    connect(client, &ClientStuff::getFilesOwner,this,&PersonalPage::getFilesOwner2);
    connect(client, &ClientStuff::getInfoUser,this,&PersonalPage::getInfoUser2);
    connect(client, &ClientStuff::getUserFiles,this,&PersonalPage::getUserFiles2);

    connect(client, &ClientStuff::getAllData,this,&PersonalPage::getAllData2);


    //every time the user push on "new file" I connect a signal
    connect(client, &ClientStuff::isFileCreated,this,&PersonalPage::isFileCreated);


    getAllData();
}

void PersonalPage::getAllData() {
    User u = User(email);
    UserMessage um = UserMessage(client->getSocket()->getClientID(),u);
    client->getSocket()->send(GET_ALL_DATA,um);
}

void PersonalPage::getAllData2(UserMessage& u,_int row1,std::vector<FilesMessage>& filesMessage1,_int row2,std::vector<FilesMessage>& filesMessage2) {
        qDebug() << "getAllData2";
        getInfoUser2(u);
        getFilesOwner2(row1,filesMessage1);
        getUserFiles2(row2,filesMessage2);
        disconnect(client, &ClientStuff::getAllData,this,&PersonalPage::getAllData2);
}


void PersonalPage::getInfoUser() {
    User u = User(email);
    UserMessage um = UserMessage(client->getSocket()->getClientID(),u);
    client->getSocket()->send(GET_INFO_USER,um);
}

void PersonalPage::getInfoUser2(UserMessage& u) {
        QFont serifFont("Times", 14, QFont::Bold);
        qDebug() << "getInfoUser2 PersonalPage";
        ui->firstName->setText(u.getUser().getEmail());
        ui->firstName->setFont(serifFont);
        ui->lastName->setText(u.getUser().getFirstName());
        ui->lastName->setFont(serifFont);
        ui->user_email->setText(u.getUser().getLastName());
        ui->user_email->setFont(serifFont);

        //now I try to retrieve data for the first table
        //getFilesOwner();
}

//retrieve files which are owned by the user
void PersonalPage::getFilesOwner() {
    User u = User(email);
    UserMessage um = UserMessage(client->getSocket()->getClientID(),u);
    client->getSocket()->send(GET_FILES_OWNER,um);
}

//receive data from mainwindow who receive data from clientstuff who recevice data from the server
void PersonalPage::getFilesOwner2(_int row, std::vector<FilesMessage>& filesMessage) {
           qDebug() << "getFilesOwner2";
           ui->tableWidget->clearContents();

           ui->tableWidget->setRowCount(row);
           QTableWidgetItem * temp;
           int i=0;
           for(auto item: filesMessage) {
               item.printUserInfo();
               temp = new QTableWidgetItem(item.getFilename());
               temp->setFlags(Qt::ItemIsEnabled);
               ui->tableWidget->setItem(i,0,temp);
               temp = new QTableWidgetItem(item.getLast_access());
               temp->setFlags(Qt::ItemIsEnabled);
               ui->tableWidget->setItem(i,1,temp);

               temp = new QTableWidgetItem(item.getCollaborators().join(" , "));
               temp->setFlags(Qt::ItemIsEnabled);
               ui->tableWidget->setItem(i,2,temp);
               i++;
           }

           //now I try to retrieve data for the second table
           //getUserFiles();
}

void PersonalPage::getUserFiles() {
    User u = User(email);
    UserMessage um = UserMessage(client->getSocket()->getClientID(),u);
    client->getSocket()->send(GET_FILES_COLLABORATOR,um);
}

void PersonalPage::getUserFiles2(_int row, std::vector<FilesMessage>& filesMessage) {
        qDebug() << "getUserFiles2 ";
        ui->tableWidget_2->clearContents();
        ui->tableWidget_2->setRowCount(row);
        QTableWidgetItem * temp;
        int i=0;
        for(auto item: filesMessage) {
            item.printUserInfo();
            temp = new QTableWidgetItem(item.getFilename());
            temp->setFlags(Qt::ItemIsEnabled);
            ui->tableWidget_2->setItem(i,0,temp);
            temp = new QTableWidgetItem(item.getOwner());
            temp->setFlags(Qt::ItemIsEnabled);
            ui->tableWidget_2->setItem(i,1,temp);

            temp = new QTableWidgetItem(item.getCollaborators().join(" , "));
            temp->setFlags(Qt::ItemIsEnabled);
            ui->tableWidget_2->setItem(i,2,temp);

            temp = new QTableWidgetItem(item.getLast_access());
            temp->setFlags(Qt::ItemIsEnabled);
            ui->tableWidget_2->setItem(i,3,temp);

            i++;
        }
}

/*
void PersonalPage::databaseManagement(QString dbPath) {
    db = QSqlDatabase::addDatabase("QSQLITE"); //"SQLITE"
    db.setDatabaseName(dbPath);
}
*/

//first table: file created by the user
void PersonalPage::on_tableWidget_cellDoubleClicked(int row, int column)
{
    //cell is different from item
    if(ui->tableWidget->item(row,column)==nullptr) {
        QMessageBox::warning(this,"Warning","NULL");
    } else if(column==0) {
        renameOrDelete *newRenameOrDelete = new renameOrDelete(this);
        connect(this,SIGNAL(sendData_2(ClientStuff *,QString,QString)),newRenameOrDelete,SLOT(receiveData_2(ClientStuff *,QString,QString)));
        emit sendData_2(this->client,email,ui->tableWidget->item(row,column)->text());

        newRenameOrDelete->show();
        qDebug() << ui->tableWidget->item(row,column)->text();
        qDebug() << "RENAME OR DELETE FILE";

        //close renameordelete and refresh table files
        connect(newRenameOrDelete,SIGNAL(Want2Close2()),this,SLOT(getFilesOwner()));
    }
    else if(column==1) {
        //TODO add something else?
    }
    else if(column==2) {
        manageCollaborators *manageC = new manageCollaborators(this);
        connect(this,SIGNAL(sendData_2(ClientStuff *,QString,QString)),manageC,SLOT(receiveData_2(ClientStuff *,QString,QString)));
        emit sendData_2(this->client,email,ui->tableWidget->item(row,0)->text());

        manageC->show();
        qDebug() << ui->tableWidget->item(row,0)->text();
        qDebug() << "ADD OR REMOVE COLLABORATOR";

        //QMessageBox::warning(this,"Attention","ADD OR REMOVE COLLABORATOR");
        connect(manageC,SIGNAL(Want2Close2()),this,SLOT(getFilesOwner()));
    }
}

void PersonalPage::on_actionChange_Password_triggered()
{

    ChangePasswordDialog *newPsw = new ChangePasswordDialog(this);
    connect(this,SIGNAL(sendData(ClientStuff *,QString)),newPsw,SLOT(receiveData(ClientStuff *, QString)));
    emit sendData(client,email);

    newPsw->show();
}

void PersonalPage::on_actionDefault_triggered()
{
    QString theme = "background: #";
    theme.append("eff0f6");
    this->setStyleSheet(theme);
    qDebug() << "theme " << theme;
}

void PersonalPage::on_actionDark_triggered()
{
    QString theme = "background: #";
    theme.append("2d383c");
    this->setStyleSheet(theme);
    qDebug() << "theme " << theme;
}

void PersonalPage::on_actionLight_triggered()
{
    QString theme = "background: #";
    theme.append("fffefc");
    this->setStyleSheet(theme);
    qDebug() << "theme " << theme;
}

//creation of new file: insert into files, control if there is a equal filename
void PersonalPage::newFile(QString email, QString filename) {

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    FileManagementMessage fileManagementMessage =
            FileManagementMessage(client->getSocket()->getClientID(),email,
                                  filename,
                                  ""); //password is not used to create a new file. Change??
    client->getSocket()->send(NEW_FILE,fileManagementMessage);

    qDebug() << "delete file push button clicked " << fileManagementMessage.getEmail();


}

void PersonalPage::on_newFile_clicked()
{
    QString filename = ui->filename->text();

    if(filename.isEmpty())
        QMessageBox::warning(this,"Error","Filename is empty");
    else
        newFile(this->email,filename);
}


void PersonalPage::on_filename_returnPressed()
{
    on_newFile_clicked();
}

void PersonalPage::isFileCreated(bool res) {
    if(res) {
              ui->filename->setText("");
                //update first table
                getFilesOwner();
    }
    else {
        QMessageBox::warning(this,"Error","Error creating file");
        disconnect(client, &ClientStuff::isFileCreated,this,&PersonalPage::isFileCreated);
    }

}

void PersonalPage::isRequestToCollaboratedReceived(bool res) {
    ui->invitationCode->setText("");
    if(res) {
        QMessageBox::information(this,"Done","Now you are a collaborator, check your files");
        disconnect(client, &ClientStuff::isRequestToCollaboratedReceived,this,&PersonalPage::isRequestToCollaboratedReceived);
        getUserFiles();
    } else {
        QMessageBox::warning(this,"Error","Error while adding you as collaborator, check your url and try again");
        disconnect(client, &ClientStuff::isRequestToCollaboratedReceived,this,&PersonalPage::isRequestToCollaboratedReceived);
    }

}


void PersonalPage::on_tableWidget_2_cellDoubleClicked(int row, int column)
{
    if(ui->tableWidget_2->item(row,column)==nullptr) {
        QMessageBox::warning(this,"Warning","NULL");
    }
    //for every column
    unsubscribeFile *unsubscribe = new unsubscribeFile(this);
    connect(this,SIGNAL(sendData_3(ClientStuff *,QString,QString,QString)),unsubscribe,SLOT(receiveData_2(ClientStuff *,QString,QString,QString)));
    emit sendData_3(this->client,email,ui->tableWidget_2->item(row,0)->text(),ui->tableWidget_2->item(row,1)->text());

    unsubscribe->show();
    qDebug() << ui->tableWidget_2->item(row,0)->text();
    qDebug() << "REMOVE MYSELF FROM A FILE";

    connect(unsubscribe,SIGNAL(Want2Close2()),this,SLOT(getUserFiles()));
}

void PersonalPage::on_requestToCollaborate_clicked()
{
    QString invitationCode = ui->invitationCode->text();

    if(invitationCode.isEmpty())
        QMessageBox::warning(this,"Error","Invitation code is empty");
    else
        requestToCollaborate(this->email,invitationCode);
}


void PersonalPage::on_invitationCode_returnPressed()
{
    on_requestToCollaborate_clicked();
}

void PersonalPage::requestToCollaborate(QString email, QString invitationCode) {
    connect(client, &ClientStuff::isRequestToCollaboratedReceived,this,&PersonalPage::isRequestToCollaboratedReceived);

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    InvitationMessage invitationMessage =
            InvitationMessage(client->getSocket()->getClientID(),email,
                                  invitationCode);
    client->getSocket()->send(REQUEST_TO_COLLABORATE,invitationMessage);

    qDebug() << "requestToCollaborate" ;

}