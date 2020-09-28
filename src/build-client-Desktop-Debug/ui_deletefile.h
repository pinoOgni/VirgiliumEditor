/********************************************************************************
** Form generated from reading UI file 'deletefile.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DELETEFILE_H
#define UI_DELETEFILE_H

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

class Ui_deleteFile
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QLineEdit *password;
    QLabel *label;
    QPushButton *pushButton;
    QLabel *label_2;

    void setupUi(QDialog *deleteFile)
    {
        if (deleteFile->objectName().isEmpty())
            deleteFile->setObjectName(QStringLiteral("deleteFile"));
        deleteFile->resize(581, 316);
        widget = new QWidget(deleteFile);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(30, 50, 461, 171));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        password = new QLineEdit(widget);
        password->setObjectName(QStringLiteral("password"));
        password->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(password, 2, 0, 1, 1);

        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setPointSize(16);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);


        retranslateUi(deleteFile);

        QMetaObject::connectSlotsByName(deleteFile);
    } // setupUi

    void retranslateUi(QDialog *deleteFile)
    {
        deleteFile->setWindowTitle(QApplication::translate("deleteFile", "Dialog", Q_NULLPTR));
        password->setPlaceholderText(QApplication::translate("deleteFile", "Password", Q_NULLPTR));
        label->setText(QString());
        pushButton->setText(QApplication::translate("deleteFile", "Delete", Q_NULLPTR));
        label_2->setText(QApplication::translate("deleteFile", "Digit password", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class deleteFile: public Ui_deleteFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DELETEFILE_H
