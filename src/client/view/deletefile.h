//
// Created by pinoOgni on 10/08/20.
//

#ifndef DELETEFILE_H
#define DELETEFILE_H

#include "renameordelete.h"



namespace Ui {
class deleteFile;
}

class deleteFile : public QDialog
{
    Q_OBJECT

public:
    explicit deleteFile(QWidget *parent = nullptr);
    ~deleteFile() override;

private:
    Ui::deleteFile *ui;

    QString email;
    QString filename;
    ClientStuff * client;

signals:
        void Want2Close3();
private slots:
        void keyPressEvent(QKeyEvent *) override;
        void receiveData_2(ClientStuff *,QString,QString);
        void on_pushButton_clicked();

        void isFileDeleted(bool);
};

#endif // DELETEFILE_H
