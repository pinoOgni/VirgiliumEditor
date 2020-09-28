//
// Created by pinoOgni on 10/08/20.
//

#ifndef RENAMEFILE_H
#define RENAMEFILE_H

#include "renameordelete.h"

namespace Ui {
class renameFile;
}

class renameFile : public QDialog
{
    Q_OBJECT

public:
    explicit renameFile(QWidget *parent = nullptr);
    ~renameFile() override;

private:
    Ui::renameFile *ui;

    QString email;
    QString oldFilename;
    ClientStuff * client;

signals:
        void Want2Close3();

private slots:
        void keyPressEvent(QKeyEvent *) override;
        void receiveData_2(ClientStuff *,QString,QString);

        void on_pushButton_clicked();
        void isFileRenamed(bool);
};

#endif // RENAMEFILE_H
