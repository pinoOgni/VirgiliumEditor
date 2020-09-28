/********************************************************************************
** Form generated from reading UI file 'changepassworddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANGEPASSWORDDIALOG_H
#define UI_CHANGEPASSWORDDIALOG_H

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

class Ui_ChangePasswordDialog
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QLineEdit *old_password;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *new_password;
    QWidget *widget1;
    QGridLayout *gridLayout_2;
    QPushButton *cancel;
    QPushButton *ok;

    void setupUi(QDialog *ChangePasswordDialog)
    {
        if (ChangePasswordDialog->objectName().isEmpty())
            ChangePasswordDialog->setObjectName(QStringLiteral("ChangePasswordDialog"));
        ChangePasswordDialog->resize(491, 180);
        widget = new QWidget(ChangePasswordDialog);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(30, 10, 401, 71));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        old_password = new QLineEdit(widget);
        old_password->setObjectName(QStringLiteral("old_password"));
        old_password->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(old_password, 1, 0, 1, 1);

        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        new_password = new QLineEdit(widget);
        new_password->setObjectName(QStringLiteral("new_password"));
        new_password->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(new_password, 1, 1, 1, 1);

        widget1 = new QWidget(ChangePasswordDialog);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(310, 140, 168, 27));
        gridLayout_2 = new QGridLayout(widget1);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        cancel = new QPushButton(widget1);
        cancel->setObjectName(QStringLiteral("cancel"));

        gridLayout_2->addWidget(cancel, 0, 0, 1, 1);

        ok = new QPushButton(widget1);
        ok->setObjectName(QStringLiteral("ok"));

        gridLayout_2->addWidget(ok, 0, 1, 1, 1);


        retranslateUi(ChangePasswordDialog);

        QMetaObject::connectSlotsByName(ChangePasswordDialog);
    } // setupUi

    void retranslateUi(QDialog *ChangePasswordDialog)
    {
        ChangePasswordDialog->setWindowTitle(QApplication::translate("ChangePasswordDialog", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("ChangePasswordDialog", "Old Password", Q_NULLPTR));
        label_2->setText(QApplication::translate("ChangePasswordDialog", "New Password", Q_NULLPTR));
        cancel->setText(QApplication::translate("ChangePasswordDialog", "Cancel", Q_NULLPTR));
        ok->setText(QApplication::translate("ChangePasswordDialog", "Ok", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ChangePasswordDialog: public Ui_ChangePasswordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGEPASSWORDDIALOG_H
