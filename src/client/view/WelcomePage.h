//
// Created by pinoOgni on 10/08/20.
//

#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QMainWindow>
#include "PersonalPage.h"
#include <QPixmap>
#include <QMessageBox>
#include <QDebug>
#include <QObject>

#include "../clientstuff.h"

namespace Ui {
class WelcomePage;
}

class WelcomePage : public QMainWindow
{
    Q_OBJECT

public:
    explicit WelcomePage(QWidget *parent = 0);
    ~WelcomePage();

    //later they will be on the server
    void loginDB(QString, QString);
    void signinDB(QString,QString,QString,QString);

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

     void getFilesOwner(int row,std::vector<FilesMessage>& userinfo);
     void getInfoUser(UserMessage& u);
     void getUserFiles(int row,std::vector<FilesMessage>& userinfo);

signals:
    void sendData(QString,ClientStuff*);
    void getFilesOwner2(int row,std::vector<FilesMessage>& userinfo);
    void getInfoUser2(UserMessage& u);
    void getUserFiles2(int row,std::vector<FilesMessage>& userinfo);


private:
    Ui::WelcomePage *ui;

    PersonalPage *personalPage;


    ClientStuff *client;


};

#endif // WELCOMEPAGE_H




