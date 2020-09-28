/********************************************************************************
** Form generated from reading UI file 'managecollaborators.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANAGECOLLABORATORS_H
#define UI_MANAGECOLLABORATORS_H

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

class Ui_manageCollaborators
{
public:
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpacerItem *verticalSpacer_2;
    QLineEdit *add_collaborator;
    QLineEdit *add_password;
    QPushButton *add;
    QSpacerItem *verticalSpacer;
    QLineEdit *remove_collaborator;
    QLineEdit *remove_password;
    QPushButton *remove;
    QPushButton *cancel;

    void setupUi(QDialog *manageCollaborators)
    {
        if (manageCollaborators->objectName().isEmpty())
            manageCollaborators->setObjectName(QStringLiteral("manageCollaborators"));
        manageCollaborators->resize(680, 393);
        layoutWidget = new QWidget(manageCollaborators);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 30, 621, 261));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 3);

        verticalSpacer_2 = new QSpacerItem(518, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 1, 0, 1, 3);

        add_collaborator = new QLineEdit(layoutWidget);
        add_collaborator->setObjectName(QStringLiteral("add_collaborator"));

        gridLayout->addWidget(add_collaborator, 2, 0, 1, 1);

        add_password = new QLineEdit(layoutWidget);
        add_password->setObjectName(QStringLiteral("add_password"));
        add_password->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(add_password, 2, 1, 1, 1);

        add = new QPushButton(layoutWidget);
        add->setObjectName(QStringLiteral("add"));

        gridLayout->addWidget(add, 2, 2, 1, 1);

        verticalSpacer = new QSpacerItem(518, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 0, 1, 3);

        remove_collaborator = new QLineEdit(layoutWidget);
        remove_collaborator->setObjectName(QStringLiteral("remove_collaborator"));

        gridLayout->addWidget(remove_collaborator, 4, 0, 1, 1);

        remove_password = new QLineEdit(layoutWidget);
        remove_password->setObjectName(QStringLiteral("remove_password"));
        remove_password->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(remove_password, 4, 1, 1, 1);

        remove = new QPushButton(layoutWidget);
        remove->setObjectName(QStringLiteral("remove"));

        gridLayout->addWidget(remove, 4, 2, 1, 1);

        cancel = new QPushButton(manageCollaborators);
        cancel->setObjectName(QStringLiteral("cancel"));
        cancel->setGeometry(QRect(550, 350, 111, 25));

        retranslateUi(manageCollaborators);

        QMetaObject::connectSlotsByName(manageCollaborators);
    } // setupUi

    void retranslateUi(QDialog *manageCollaborators)
    {
        manageCollaborators->setWindowTitle(QApplication::translate("manageCollaborators", "Dialog", Q_NULLPTR));
        label->setText(QString());
        add_collaborator->setPlaceholderText(QApplication::translate("manageCollaborators", "Add collaborator", Q_NULLPTR));
        add_password->setPlaceholderText(QApplication::translate("manageCollaborators", "Password", Q_NULLPTR));
        add->setText(QApplication::translate("manageCollaborators", "Add", Q_NULLPTR));
        remove_collaborator->setPlaceholderText(QApplication::translate("manageCollaborators", "Remove collaborator", Q_NULLPTR));
        remove_password->setPlaceholderText(QApplication::translate("manageCollaborators", "Password", Q_NULLPTR));
        remove->setText(QApplication::translate("manageCollaborators", "Remove", Q_NULLPTR));
        cancel->setText(QApplication::translate("manageCollaborators", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class manageCollaborators: public Ui_manageCollaborators {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANAGECOLLABORATORS_H
