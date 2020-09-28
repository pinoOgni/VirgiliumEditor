/********************************************************************************
** Form generated from reading UI file 'renamefile.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENAMEFILE_H
#define UI_RENAMEFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_renameFile
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QLabel *label;
    QLineEdit *password;
    QLineEdit *newFilename;

    void setupUi(QDialog *renameFile)
    {
        if (renameFile->objectName().isEmpty())
            renameFile->setObjectName(QStringLiteral("renameFile"));
        renameFile->resize(659, 343);
        widget = new QWidget(renameFile);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(50, 50, 521, 191));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 1, 2, 1, 1);

        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 3);

        password = new QLineEdit(widget);
        password->setObjectName(QStringLiteral("password"));
        password->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(password, 1, 1, 1, 1);

        newFilename = new QLineEdit(widget);
        newFilename->setObjectName(QStringLiteral("newFilename"));

        gridLayout->addWidget(newFilename, 1, 0, 1, 1);


        retranslateUi(renameFile);

        QMetaObject::connectSlotsByName(renameFile);
    } // setupUi

    void retranslateUi(QDialog *renameFile)
    {
        renameFile->setWindowTitle(QApplication::translate("renameFile", "Dialog", Q_NULLPTR));
        pushButton->setText(QApplication::translate("renameFile", "Rename", Q_NULLPTR));
        label->setText(QString());
        password->setPlaceholderText(QApplication::translate("renameFile", "Password", Q_NULLPTR));
        newFilename->setPlaceholderText(QApplication::translate("renameFile", "New filename", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class renameFile: public Ui_renameFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENAMEFILE_H
