/********************************************************************************
** Form generated from reading UI file 'unsubscribefile.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNSUBSCRIBEFILE_H
#define UI_UNSUBSCRIBEFILE_H

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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_unsubscribeFile
{
public:
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLineEdit *password;
    QPushButton *unsubscribe;
    QLabel *label;
    QSpacerItem *verticalSpacer;
    QPushButton *cancel;

    void setupUi(QDialog *unsubscribeFile)
    {
        if (unsubscribeFile->objectName().isEmpty())
            unsubscribeFile->setObjectName(QStringLiteral("unsubscribeFile"));
        unsubscribeFile->resize(640, 399);
        layoutWidget = new QWidget(unsubscribeFile);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(90, 130, 381, 141));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        password = new QLineEdit(layoutWidget);
        password->setObjectName(QStringLiteral("password"));
        password->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(password, 2, 0, 1, 1);

        unsubscribe = new QPushButton(layoutWidget);
        unsubscribe->setObjectName(QStringLiteral("unsubscribe"));

        gridLayout->addWidget(unsubscribe, 2, 1, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 2);

        verticalSpacer = new QSpacerItem(278, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 2);

        cancel = new QPushButton(unsubscribeFile);
        cancel->setObjectName(QStringLiteral("cancel"));
        cancel->setGeometry(QRect(500, 350, 89, 25));

        retranslateUi(unsubscribeFile);

        QMetaObject::connectSlotsByName(unsubscribeFile);
    } // setupUi

    void retranslateUi(QDialog *unsubscribeFile)
    {
        unsubscribeFile->setWindowTitle(QApplication::translate("unsubscribeFile", "Dialog", Q_NULLPTR));
        password->setPlaceholderText(QApplication::translate("unsubscribeFile", "Password", Q_NULLPTR));
        unsubscribe->setText(QApplication::translate("unsubscribeFile", "Unsubscribe", Q_NULLPTR));
        label->setText(QString());
        cancel->setText(QApplication::translate("unsubscribeFile", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class unsubscribeFile: public Ui_unsubscribeFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNSUBSCRIBEFILE_H
