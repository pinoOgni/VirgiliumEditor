//
// Created by pinoOgni on 10/08/20.
//

#ifndef VIRGILIUM_RENAMEFILE_H
#define VIRGILIUM_RENAMEFILE_H

#include "renameordelete.h"

namespace Ui {
    class renameFile;
}

class renameFile : public QDialog {
Q_OBJECT

public:
    explicit renameFile(QWidget *parent = nullptr);

    ~renameFile() override;

private:
    Ui::renameFile *ui;

    QString email;
    QString oldFilename;
    ClientStuff *client;

signals:
    /*
     * signal between renameFile and renameordelete
     */
    void Want2Close3();

private slots:

    /*
     * override to manage the keypressevent
     */
    void keyPressEvent(QKeyEvent *) override;

    /*
     * receive data from PersonalPage
     */
    void receiveData_2(ClientStuff *, QString, QString);

    /*
     * this slot is connected with isFileRenamed signal of clientStuff
     */
    void isFileRenamed(bool);

    void on_pushButton_clicked();

};

#endif // VIRGILIUM_RENAMEFILE_H
