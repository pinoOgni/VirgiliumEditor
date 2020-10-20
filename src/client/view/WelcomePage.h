//
// Created by pinoOgni on 10/08/20.
//

#ifndef VIRGILIUM_WELCOMEPAGE_H
#define VIRGILIUM_WELCOMEPAGE_H

#include <QMainWindow>
#include "PersonalPage.h"
#include <QPixmap>
#include <QMessageBox>
#include <QDebug>
#include <QObject>
#include <QtCore/QRegularExpression>

#include "../clientstuff.h"

namespace Ui {
    class WelcomePage;
}

class WelcomePage : public QMainWindow {
Q_OBJECT

public:
    explicit WelcomePage(QWidget *parent = 0);

    ~WelcomePage();

    void loginDB(QString, QString);
    void signinDB(QString, QString, QString, QString);

private slots:

    void on_accedi_clicked();

    bool validaEmail(QString);

    bool validaNome_Cognome(QString);

    void on_iscriviti_clicked();

    void on_password_accedi_returnPressed();

    void on_password_iscriviti_returnPressed();

public slots:

    void loggedIn(const bool logged);

    void registered(const bool registered);

    void userAlreadyLogged();

    void getFilesOwner(int row, std::vector<FilesMessage> &userinfo);

    void getInfoUser(UserMessage &u);

    void getUserFiles(int row, std::vector<FilesMessage> &userinfo);

signals:

    void sendData(QString, ClientStuff *);

    void getFilesOwner2(int row, std::vector<FilesMessage> &userinfo);

    void getInfoUser2(UserMessage &u);

    void getUserFiles2(int row, std::vector<FilesMessage> &userinfo);


private:
    Ui::WelcomePage *ui;

    PersonalPage *personalPage;


    ClientStuff *client;


};

#endif // VIRGILIUM_WELCOMEPAGE_H




