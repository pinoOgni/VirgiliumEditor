/********************************************************************************
** Form generated from reading UI file 'renameordelete.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENAMEORDELETE_H
#define UI_RENAMEORDELETE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_renameOrDelete
{
public:
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QGridLayout *gridLayout_2;
    QPushButton *rename;
    QPushButton *delete_2;
    QPushButton *openTextEditor;
    QPushButton *cancel;

    void setupUi(QDialog *renameOrDelete)
    {
        if (renameOrDelete->objectName().isEmpty())
            renameOrDelete->setObjectName(QStringLiteral("renameOrDelete"));
        renameOrDelete->resize(820, 463);
        layoutWidget = new QWidget(renameOrDelete);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(90, 70, 578, 251));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setPointSize(18);
        font.setItalic(true);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        rename = new QPushButton(layoutWidget);
        rename->setObjectName(QStringLiteral("rename"));

        gridLayout_2->addWidget(rename, 0, 0, 1, 1);

        delete_2 = new QPushButton(layoutWidget);
        delete_2->setObjectName(QStringLiteral("delete_2"));

        gridLayout_2->addWidget(delete_2, 0, 1, 1, 1);

        openTextEditor = new QPushButton(layoutWidget);
        openTextEditor->setObjectName(QStringLiteral("openTextEditor"));

        gridLayout_2->addWidget(openTextEditor, 1, 0, 1, 2);


        gridLayout->addLayout(gridLayout_2, 1, 0, 1, 2);

        cancel = new QPushButton(renameOrDelete);
        cancel->setObjectName(QStringLiteral("cancel"));
        cancel->setGeometry(QRect(670, 410, 89, 25));

        retranslateUi(renameOrDelete);

        QMetaObject::connectSlotsByName(renameOrDelete);
    } // setupUi

    void retranslateUi(QDialog *renameOrDelete)
    {
        renameOrDelete->setWindowTitle(QApplication::translate("renameOrDelete", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("renameOrDelete", "                          What do you want to do?", Q_NULLPTR));
        rename->setText(QApplication::translate("renameOrDelete", "Rename", Q_NULLPTR));
        delete_2->setText(QApplication::translate("renameOrDelete", "Delete", Q_NULLPTR));
        openTextEditor->setText(QApplication::translate("renameOrDelete", "Open text editor", Q_NULLPTR));
        cancel->setText(QApplication::translate("renameOrDelete", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class renameOrDelete: public Ui_renameOrDelete {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENAMEORDELETE_H
