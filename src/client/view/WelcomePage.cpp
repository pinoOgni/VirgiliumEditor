//
// Created by pinoOgni on 10/08/20.
//

#include <QtCore/QRegularExpression>
#include "WelcomePage.h"
#include "../../../cmake-build-debug/VirgiliumClient_autogen/include/ui_WelcomePage.h"


WelcomePage::WelcomePage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WelcomePage)
{
    ui->setupUi(this);

    //setup style
    this->setStyleSheet("background: #eff0f6;");
    QPixmap pix(":/Icons/virgilium.png");
    int w = ui->label_3->width();
    int h = ui->label_3->height();
    ui->label_3->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

    client = new ClientStuff("127.0.0.1", LISTENING_PORT);

    connect(client, &ClientStuff::tryToLogin, this, &WelcomePage::loggedIn);
    connect(client, &ClientStuff::tryToSignup, this, &WelcomePage::registered);
}


WelcomePage::~WelcomePage()
{
    delete client;
    delete ui;
}

void WelcomePage::loggedIn(const bool logged) {
    qDebug() << "loggedIn " << logged;
    if(logged) {
        QMessageBox::information(this,"Login","Logged in");
        QString email = ui->email_accedi->text();

        personalPage = new PersonalPage;
        personalPage->setAttribute(Qt::WA_DeleteOnClose);
        connect(this, SIGNAL(sendData(QString, ClientStuff * )), personalPage,
                SLOT(receiveData(QString, ClientStuff * )));
        connect(personalPage, &PersonalPage::Want2Close, this, &WelcomePage::show);
        emit sendData(email, client);

        ui->email_accedi->setText("");
        ui->password_accedi->setText("");

        close();
        personalPage->show();
    } else {
        QMessageBox::warning(this, "Login", "Incorrect email or password, please try again");
    }
}


void WelcomePage::registered(const bool registered) {
    if(registered) {
        ui->email_iscriviti->setText("");
        ui->password_iscriviti->setText("");
        ui->nome_iscriviti->setText("");
        ui->cognome_iscriviti->setText("");
        QMessageBox::information(this,"Iscriviti","Now you can login");
    } else {
        QMessageBox::information(this,"Iscriviti","Email already in the DB");
        ui->email_iscriviti->setText("");
        ui->password_iscriviti->setText("");
    }
}

void WelcomePage::getFilesOwner(int row,std::vector<FilesMessage>& filesMessage) {
    qDebug() << "getFilesOwner welcomepage";
    emit getFilesOwner2(row,filesMessage);
}

void WelcomePage::getInfoUser(UserMessage& um) {
    qDebug() << um.getUser().printMessage() << "getInfoUser welcomepage";
    emit getInfoUser2(um);
}

void WelcomePage::getUserFiles(int row,std::vector<FilesMessage>& filesMessage) {
    qDebug() << "getUserFiles welcomepage";

    emit getUserFiles2(row,filesMessage);
}

void WelcomePage::on_accedi_clicked()
{
    QString email = ui->email_accedi->text();
    QString password = ui->password_accedi->text();

    qDebug() << email << password;

    if(email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this,"Error","Email and/or password empty");
    }
    else {
        if(!validaEmail(email))
            QMessageBox::warning(this,"Login","Wrong email format, please try again with email@domain");
        else {
            loginDB(email,password);
         }
    }
}

void WelcomePage::on_iscriviti_clicked()
{
    QString email = ui->email_iscriviti->text();
    QString password = ui->password_iscriviti->text();
    QString firstname = ui->nome_iscriviti->text();
    QString lastname = ui->cognome_iscriviti->text();

     qDebug() << firstname << lastname<< email << password;

    if(email.isEmpty() || password.isEmpty() || firstname.isEmpty() || lastname.isEmpty()) {
        QMessageBox::warning(this,"Error","One or more fields are empty");
    } else {
        if(!validaEmail(email))
            QMessageBox::warning(this,"Iscriviti","Formato email errato, riprova con email@dominio");
        else if(!validaNome_Cognome(firstname) || !validaNome_Cognome(lastname)) {
            QMessageBox::warning(this,"Iscriviti","Formato nome o cognome errati, solo lettere");
         }
        else {
            signinDB(email,password,firstname,lastname);
        }
    }
}


bool WelcomePage::validaEmail(QString email) {
    QRegularExpression regex("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b", QRegularExpression::CaseInsensitiveOption);
    if(!regex.match(email).hasMatch())
        return false;

    return true;
}


bool WelcomePage::validaNome_Cognome(QString valore) {
    QRegularExpression regex("^[a-zA-Z]+$", QRegularExpression::CaseInsensitiveOption);
    if(!regex.match(valore).hasMatch())
        return false;

    return true;
}


void WelcomePage::on_password_accedi_returnPressed()
{
    this->on_accedi_clicked();
}



void WelcomePage::on_password_iscriviti_returnPressed()
{
    this->on_iscriviti_clicked();
}


void WelcomePage::loginDB(QString email, QString password) {
    User u = User(email, password);
    u.setSiteId(this->client->getSocket()->getClientID());
    UserMessage um = UserMessage(client->getSocket()->getClientID(), u);
    client->getSocket()->send(LOGIN, um);
}

void WelcomePage::signinDB(QString email, QString password, QString firstname, QString lastname) {
    User u = User(email,password,firstname,lastname);
    UserMessage um = UserMessage(client->getSocket()->getClientID(),u);
    client->getSocket()->send(SIGNUP,um);
}





