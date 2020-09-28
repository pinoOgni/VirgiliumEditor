/********************************************************************************
** Form generated from reading UI file 'WelcomePage.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WELCOMEPAGE_H
#define UI_WELCOMEPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WelcomePage
{
public:
    QWidget *centralWidget;
    QLabel *label_3;
    QLabel *label_4;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QLineEdit *nome_iscriviti;
    QLineEdit *cognome_iscriviti;
    QLineEdit *email_iscriviti;
    QLineEdit *password_iscriviti;
    QPushButton *iscriviti;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QLineEdit *email_accedi;
    QLabel *label;
    QLineEdit *password_accedi;
    QPushButton *accedi;
    QLabel *label_2;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *WelcomePage)
    {
        if (WelcomePage->objectName().isEmpty())
            WelcomePage->setObjectName(QStringLiteral("WelcomePage"));
        WelcomePage->resize(1245, 674);
        centralWidget = new QWidget(WelcomePage);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(40, 0, 361, 121));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 140, 601, 41));
        QFont font;
        font.setFamily(QStringLiteral("URW Chancery L"));
        font.setPointSize(24);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        label_4->setFont(font);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(630, 320, 381, 201));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        nome_iscriviti = new QLineEdit(layoutWidget);
        nome_iscriviti->setObjectName(QStringLiteral("nome_iscriviti"));

        gridLayout_2->addWidget(nome_iscriviti, 0, 0, 1, 1);

        cognome_iscriviti = new QLineEdit(layoutWidget);
        cognome_iscriviti->setObjectName(QStringLiteral("cognome_iscriviti"));

        gridLayout_2->addWidget(cognome_iscriviti, 0, 1, 1, 1);

        email_iscriviti = new QLineEdit(layoutWidget);
        email_iscriviti->setObjectName(QStringLiteral("email_iscriviti"));

        gridLayout_2->addWidget(email_iscriviti, 1, 0, 1, 2);

        password_iscriviti = new QLineEdit(layoutWidget);
        password_iscriviti->setObjectName(QStringLiteral("password_iscriviti"));
        password_iscriviti->setEchoMode(QLineEdit::Password);

        gridLayout_2->addWidget(password_iscriviti, 2, 0, 1, 2);

        iscriviti = new QPushButton(layoutWidget);
        iscriviti->setObjectName(QStringLiteral("iscriviti"));

        gridLayout_2->addWidget(iscriviti, 3, 0, 1, 2);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(617, 40, 411, 73));
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        email_accedi = new QLineEdit(layoutWidget1);
        email_accedi->setObjectName(QStringLiteral("email_accedi"));

        gridLayout->addWidget(email_accedi, 1, 0, 1, 1);

        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        password_accedi = new QLineEdit(layoutWidget1);
        password_accedi->setObjectName(QStringLiteral("password_accedi"));
        password_accedi->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(password_accedi, 1, 1, 1, 1);

        accedi = new QPushButton(layoutWidget1);
        accedi->setObjectName(QStringLiteral("accedi"));

        gridLayout->addWidget(accedi, 1, 2, 1, 1);

        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 1, 1, 2);

        WelcomePage->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(WelcomePage);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1245, 22));
        WelcomePage->setMenuBar(menuBar);
        statusBar = new QStatusBar(WelcomePage);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        WelcomePage->setStatusBar(statusBar);

        retranslateUi(WelcomePage);

        QMetaObject::connectSlotsByName(WelcomePage);
    } // setupUi

    void retranslateUi(QMainWindow *WelcomePage)
    {
        WelcomePage->setWindowTitle(QApplication::translate("WelcomePage", "MainWindow", Q_NULLPTR));
        label_3->setText(QString());
        label_4->setText(QApplication::translate("WelcomePage", "Virgilium ti accompagner\303\240 nella stesura dei tuoi testi.", Q_NULLPTR));
        nome_iscriviti->setPlaceholderText(QApplication::translate("WelcomePage", "Nome", Q_NULLPTR));
        cognome_iscriviti->setPlaceholderText(QApplication::translate("WelcomePage", "Cognome", Q_NULLPTR));
        email_iscriviti->setPlaceholderText(QApplication::translate("WelcomePage", "E-mail", Q_NULLPTR));
        password_iscriviti->setPlaceholderText(QApplication::translate("WelcomePage", "Password", Q_NULLPTR));
        iscriviti->setText(QApplication::translate("WelcomePage", "Iscriviti", Q_NULLPTR));
        label->setText(QApplication::translate("WelcomePage", "E-mail", Q_NULLPTR));
        accedi->setText(QApplication::translate("WelcomePage", "Accedi", Q_NULLPTR));
        label_2->setText(QApplication::translate("WelcomePage", "Password", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class WelcomePage: public Ui_WelcomePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WELCOMEPAGE_H
