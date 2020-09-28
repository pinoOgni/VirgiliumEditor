/********************************************************************************
** Form generated from reading UI file 'PersonalPage.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PERSONALPAGE_H
#define UI_PERSONALPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PersonalPage
{
public:
    QAction *actionChange_Password;
    QAction *actionDefault;
    QAction *actionLight;
    QAction *actionDark;
    QWidget *centralwidget;
    QLabel *label_orario;
    QSplitter *splitter;
    QLineEdit *filename;
    QPushButton *newFile;
    QPushButton *logout;
    QLabel *label_orario_2;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *user_email;
    QLabel *lastName;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *firstName;
    QLabel *label;
    QTableWidget *tableWidget;
    QTableWidget *tableWidget_2;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menuSettings;
    QMenu *menuChange_Theme;

    void setupUi(QMainWindow *PersonalPage)
    {
        if (PersonalPage->objectName().isEmpty())
            PersonalPage->setObjectName(QStringLiteral("PersonalPage"));
        PersonalPage->resize(1224, 703);
        PersonalPage->setMouseTracking(true);
        actionChange_Password = new QAction(PersonalPage);
        actionChange_Password->setObjectName(QStringLiteral("actionChange_Password"));
        actionDefault = new QAction(PersonalPage);
        actionDefault->setObjectName(QStringLiteral("actionDefault"));
        actionLight = new QAction(PersonalPage);
        actionLight->setObjectName(QStringLiteral("actionLight"));
        actionDark = new QAction(PersonalPage);
        actionDark->setObjectName(QStringLiteral("actionDark"));
        centralwidget = new QWidget(PersonalPage);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        label_orario = new QLabel(centralwidget);
        label_orario->setObjectName(QStringLiteral("label_orario"));
        label_orario->setGeometry(QRect(10, 610, 201, 41));
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setGeometry(QRect(870, 10, 301, 31));
        splitter->setOrientation(Qt::Horizontal);
        filename = new QLineEdit(splitter);
        filename->setObjectName(QStringLiteral("filename"));
        filename->setMouseTracking(true);
        splitter->addWidget(filename);
        newFile = new QPushButton(splitter);
        newFile->setObjectName(QStringLiteral("newFile"));
        splitter->addWidget(newFile);
        logout = new QPushButton(centralwidget);
        logout->setObjectName(QStringLiteral("logout"));
        logout->setGeometry(QRect(1010, 570, 161, 51));
        label_orario_2 = new QLabel(centralwidget);
        label_orario_2->setObjectName(QStringLiteral("label_orario_2"));
        label_orario_2->setGeometry(QRect(10, 640, 181, 71));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(180, 30, 671, 561));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        user_email = new QLabel(layoutWidget);
        user_email->setObjectName(QStringLiteral("user_email"));

        gridLayout->addWidget(user_email, 1, 2, 1, 1);

        lastName = new QLabel(layoutWidget);
        lastName->setObjectName(QStringLiteral("lastName"));

        gridLayout->addWidget(lastName, 1, 1, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        firstName = new QLabel(layoutWidget);
        firstName->setObjectName(QStringLiteral("firstName"));

        gridLayout->addWidget(firstName, 1, 0, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        tableWidget = new QTableWidget(layoutWidget);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setMaximumSize(QSize(16777215, 253));

        gridLayout_2->addWidget(tableWidget, 1, 0, 1, 1);

        tableWidget_2 = new QTableWidget(layoutWidget);
        if (tableWidget_2->columnCount() < 4)
            tableWidget_2->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(3, __qtablewidgetitem6);
        tableWidget_2->setObjectName(QStringLiteral("tableWidget_2"));

        gridLayout_2->addWidget(tableWidget_2, 2, 0, 1, 1);

        PersonalPage->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(PersonalPage);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        PersonalPage->setStatusBar(statusbar);
        menubar = new QMenuBar(PersonalPage);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1224, 22));
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName(QStringLiteral("menuSettings"));
        menuChange_Theme = new QMenu(menuSettings);
        menuChange_Theme->setObjectName(QStringLiteral("menuChange_Theme"));
        PersonalPage->setMenuBar(menubar);

        menubar->addAction(menuSettings->menuAction());
        menuSettings->addAction(actionChange_Password);
        menuSettings->addAction(menuChange_Theme->menuAction());
        menuChange_Theme->addAction(actionDefault);
        menuChange_Theme->addAction(actionLight);
        menuChange_Theme->addAction(actionDark);

        retranslateUi(PersonalPage);

        QMetaObject::connectSlotsByName(PersonalPage);
    } // setupUi

    void retranslateUi(QMainWindow *PersonalPage)
    {
        PersonalPage->setWindowTitle(QApplication::translate("PersonalPage", "MainWindow", Q_NULLPTR));
        actionChange_Password->setText(QApplication::translate("PersonalPage", "Change Password", Q_NULLPTR));
        actionDefault->setText(QApplication::translate("PersonalPage", "Default", Q_NULLPTR));
        actionLight->setText(QApplication::translate("PersonalPage", "Light", Q_NULLPTR));
        actionDark->setText(QApplication::translate("PersonalPage", "Dark", Q_NULLPTR));
        label_orario->setText(QString());
        newFile->setText(QApplication::translate("PersonalPage", "New file", Q_NULLPTR));
        logout->setText(QApplication::translate("PersonalPage", "Logout", Q_NULLPTR));
        label_orario_2->setText(QString());
        user_email->setText(QString());
        lastName->setText(QString());
        label_3->setText(QApplication::translate("PersonalPage", "Last Name", Q_NULLPTR));
        label_2->setText(QApplication::translate("PersonalPage", "First Name", Q_NULLPTR));
        firstName->setText(QString());
        label->setText(QApplication::translate("PersonalPage", "Email", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("PersonalPage", "Filename", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("PersonalPage", "Last access", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("PersonalPage", "Collaborators", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("PersonalPage", "Filename", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("PersonalPage", "Owner", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_2->horizontalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("PersonalPage", "Collaborators", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_2->horizontalHeaderItem(3);
        ___qtablewidgetitem6->setText(QApplication::translate("PersonalPage", "My last access", Q_NULLPTR));
        menuSettings->setTitle(QApplication::translate("PersonalPage", "Settings", Q_NULLPTR));
        menuChange_Theme->setTitle(QApplication::translate("PersonalPage", "Change Theme", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PersonalPage: public Ui_PersonalPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PERSONALPAGE_H
