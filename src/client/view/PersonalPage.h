//
// Created by pinoOgni on 10/08/20.
//

#ifndef VIRGILIUM_PERSONALPAGE_H
#define VIRGILIUM_PERSONALPAGE_H

#include <QMainWindow>
#include <QDateTime>
#include <QTimer>
#include <QFont>
#include <QMessageBox>
#include <QDebug>
#include "../clientstuff.h"

namespace Ui {
    class PersonalPage;
}

class PersonalPage : public QMainWindow {
Q_OBJECT

public:
    explicit PersonalPage(QWidget *parent = nullptr);

    ~PersonalPage();

private slots:

    void on_logout_clicked();

    void on_newFile_clicked();

    void on_filename_returnPressed();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_actionChange_Password_triggered();

    void on_actionDefault_triggered();

    void on_actionLight_triggered();

    void on_tableWidget_2_cellDoubleClicked(int row, int column);

    void on_requestToCollaborate_clicked();

    void on_invitationCode_returnPressed();

    /*
     * used to send a file creation request to the server
     */
    void newFile(QString, QString);

    /*
     * used to send a collaboration request to a file on the server
     */
    void requestToCollaborate(QString, QString);

    /*
     * used to display the time
     */
    void time_label();

    /*
     * recevide data from WelcomePage and set some connections (signal, slot)
     */
    void receiveData(QString, ClientStuff *);

    /*
     * send a request to the server to retrieve info user (email, firstname, lastname)
     */
    void getInfoUser();

    /*
     * send a request to the server to retrieve files that the user owns
     */
    void getFilesOwner();

    /*
     * send a request to the server to retrieve files that the user collaborates with
     */
    void getUserFiles();

    /*
     * send a request to the server to retrieve all info (user's info, files)
     */
    void getAllData();

    /*
     * this slot is connected with the getInfoUser signal from clientStuff
     */
    void getInfoUser2(UserMessage &u);

    /*
     * this slot is connected with the getFilesOwner signal from clientStuff
     */
    void getFilesOwner2(_int row, std::vector<FilesMessage> &filesMessage);

    /*
     * this slot is connected with the getUserFiles signal from clientStuff
     */
    void getUserFiles2(_int row, std::vector<FilesMessage> &filesMessage);

    /*
     * this slot is connected with the isFileCreated signal from clientStuff
     */
    void isFileCreated(bool);

    /*
     * this slot is connected with the isRequestToCollaboratedReceived signal from clientStuff
     */
    void isRequestToCollaboratedReceived(bool);

    /*
     * this slot is connected with the getAllData signal from clientStuff
     */
    void getAllData2(UserMessage &, _int, std::vector<FilesMessage> &, _int, std::vector<FilesMessage> &);

    /*
     * is used to retrieve information from the server every T seconds
     */
    void updateTimerSlot();

    /*
     * override to manage the "X" button of the window
     */
    void closeEvent(QCloseEvent *);

signals:

    /*
     * signal between PersonalPage and WelcomePage
     */
    void Want2Close();

    /*
     * send data to changepassworddialog
     */
    void sendData(ClientStuff *, QString);

    /*
     * send data to deleteOrRenamedialog
     */
    void sendData_2(ClientStuff *, QString, QString, User);

    /*
     * send data to unsubscribedialog
     */
    void sendData_3(ClientStuff *, QString, QString, QString, User);

    /*
     * close the textEditor
     */
    void closeEditor();

private:
    Ui::PersonalPage *ui;
    User currentUser;
    QString email;
    QTimer *timer;
    QTimer *updateTimer;

    ClientStuff *client;

};

#endif //VIRGILIUM_PERSONALPAGE_H
