/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionNew;
    QAction *actionSave_as;
    QAction *actionCopy;
    QAction *actionCut;
    QAction *actionPaste;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionExit;
    QAction *actionRight_allignment;
    QAction *actionLeft_allignment;
    QAction *actionCenter_allignment;
    QAction *actionJustify;
    QAction *actionSelect_all;
    QAction *actionAumenta_rientro;
    QAction *actionDiminuisci_rientro;
    QAction *actionSottolinea;
    QAction *actionGrassetto;
    QAction *actionCorsivo;
    QAction *actionTrova_e_sostituisci;
    QAction *actionEsporta_PDF;
    QAction *actionTabella;
    QAction *actionImmagine;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuParagraph;
    QMenu *menuAlignment;
    QMenu *menuTesto;
    QMenu *menuInserisci;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1213, 659);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icons/Open.png"), QSize(), QIcon::Normal, QIcon::On);
        actionOpen->setIcon(icon);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Icons/New.png"), QSize(), QIcon::Normal, QIcon::On);
        actionNew->setIcon(icon1);
        actionSave_as = new QAction(MainWindow);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Icons/Save.png"), QSize(), QIcon::Normal, QIcon::On);
        actionSave_as->setIcon(icon2);
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Icons/Copy.png"), QSize(), QIcon::Normal, QIcon::On);
        actionCopy->setIcon(icon3);
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Icons/Cut.png"), QSize(), QIcon::Normal, QIcon::On);
        actionCut->setIcon(icon4);
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/Icons/Paste.png"), QSize(), QIcon::Normal, QIcon::On);
        actionPaste->setIcon(icon5);
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/Icons/undo.png"), QSize(), QIcon::Normal, QIcon::On);
        actionUndo->setIcon(icon6);
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/Icons/redo.png"), QSize(), QIcon::Normal, QIcon::On);
        actionRedo->setIcon(icon7);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/Icons/close.png"), QSize(), QIcon::Normal, QIcon::On);
        actionExit->setIcon(icon8);
        actionRight_allignment = new QAction(MainWindow);
        actionRight_allignment->setObjectName(QString::fromUtf8("actionRight_allignment"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/Icons/right-align.png"), QSize(), QIcon::Normal, QIcon::On);
        actionRight_allignment->setIcon(icon9);
        actionLeft_allignment = new QAction(MainWindow);
        actionLeft_allignment->setObjectName(QString::fromUtf8("actionLeft_allignment"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/Icons/left-align.png"), QSize(), QIcon::Normal, QIcon::On);
        actionLeft_allignment->setIcon(icon10);
        actionCenter_allignment = new QAction(MainWindow);
        actionCenter_allignment->setObjectName(QString::fromUtf8("actionCenter_allignment"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/Icons/center-align.png"), QSize(), QIcon::Normal, QIcon::On);
        actionCenter_allignment->setIcon(icon11);
        actionJustify = new QAction(MainWindow);
        actionJustify->setObjectName(QString::fromUtf8("actionJustify"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/Icons/justify.png"), QSize(), QIcon::Normal, QIcon::On);
        actionJustify->setIcon(icon12);
        actionSelect_all = new QAction(MainWindow);
        actionSelect_all->setObjectName(QString::fromUtf8("actionSelect_all"));
        actionAumenta_rientro = new QAction(MainWindow);
        actionAumenta_rientro->setObjectName(QString::fromUtf8("actionAumenta_rientro"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/Icons/indent.png"), QSize(), QIcon::Normal, QIcon::On);
        actionAumenta_rientro->setIcon(icon13);
        actionDiminuisci_rientro = new QAction(MainWindow);
        actionDiminuisci_rientro->setObjectName(QString::fromUtf8("actionDiminuisci_rientro"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/Icons/outdent.png"), QSize(), QIcon::Normal, QIcon::On);
        actionDiminuisci_rientro->setIcon(icon14);
        actionSottolinea = new QAction(MainWindow);
        actionSottolinea->setObjectName(QString::fromUtf8("actionSottolinea"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/Icons/underline.png"), QSize(), QIcon::Normal, QIcon::On);
        actionSottolinea->setIcon(icon15);
        actionGrassetto = new QAction(MainWindow);
        actionGrassetto->setObjectName(QString::fromUtf8("actionGrassetto"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/Icons/bold.png"), QSize(), QIcon::Normal, QIcon::On);
        actionGrassetto->setIcon(icon16);
        actionCorsivo = new QAction(MainWindow);
        actionCorsivo->setObjectName(QString::fromUtf8("actionCorsivo"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/Icons/italic.png"), QSize(), QIcon::Normal, QIcon::On);
        actionCorsivo->setIcon(icon17);
        actionTrova_e_sostituisci = new QAction(MainWindow);
        actionTrova_e_sostituisci->setObjectName(QString::fromUtf8("actionTrova_e_sostituisci"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/Icons/preview.png"), QSize(), QIcon::Normal, QIcon::On);
        actionTrova_e_sostituisci->setIcon(icon18);
        actionEsporta_PDF = new QAction(MainWindow);
        actionEsporta_PDF->setObjectName(QString::fromUtf8("actionEsporta_PDF"));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/Icons/pdf.png"), QSize(), QIcon::Normal, QIcon::On);
        actionEsporta_PDF->setIcon(icon19);
        actionTabella = new QAction(MainWindow);
        actionTabella->setObjectName(QString::fromUtf8("actionTabella"));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/Icons/table.png"), QSize(), QIcon::Normal, QIcon::On);
        actionTabella->setIcon(icon20);
        actionImmagine = new QAction(MainWindow);
        actionImmagine->setObjectName(QString::fromUtf8("actionImmagine"));
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/Icons/photograph.png"), QSize(), QIcon::Normal, QIcon::On);
        actionImmagine->setIcon(icon21);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        QFont font;
        font.setPointSize(12);
        textEdit->setFont(font);

        verticalLayout->addWidget(textEdit);


        verticalLayout_2->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1213, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuParagraph = new QMenu(menubar);
        menuParagraph->setObjectName(QString::fromUtf8("menuParagraph"));
        menuAlignment = new QMenu(menuParagraph);
        menuAlignment->setObjectName(QString::fromUtf8("menuAlignment"));
        menuTesto = new QMenu(menuParagraph);
        menuTesto->setObjectName(QString::fromUtf8("menuTesto"));
        menuInserisci = new QMenu(menubar);
        menuInserisci->setObjectName(QString::fromUtf8("menuInserisci"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuParagraph->menuAction());
        menubar->addAction(menuInserisci->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionNew);
        menuFile->addAction(actionSave_as);
        menuFile->addAction(actionEsporta_PDF);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionPaste);
        menuEdit->addAction(actionSelect_all);
        menuEdit->addSeparator();
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionTrova_e_sostituisci);
        menuParagraph->addAction(menuTesto->menuAction());
        menuParagraph->addAction(menuAlignment->menuAction());
        menuAlignment->addAction(actionLeft_allignment);
        menuAlignment->addAction(actionRight_allignment);
        menuAlignment->addAction(actionCenter_allignment);
        menuAlignment->addAction(actionJustify);
        menuAlignment->addSeparator();
        menuAlignment->addAction(actionAumenta_rientro);
        menuAlignment->addAction(actionDiminuisci_rientro);
        menuTesto->addAction(actionSottolinea);
        menuTesto->addAction(actionGrassetto);
        menuTesto->addAction(actionCorsivo);
        menuInserisci->addAction(actionTabella);
        menuInserisci->addAction(actionImmagine);
        toolBar->addAction(actionNew);
        toolBar->addAction(actionOpen);
        toolBar->addAction(actionSave_as);
        toolBar->addAction(actionEsporta_PDF);
        toolBar->addSeparator();
        toolBar->addAction(actionCopy);
        toolBar->addAction(actionPaste);
        toolBar->addAction(actionCut);
        toolBar->addAction(actionUndo);
        toolBar->addAction(actionRedo);
        toolBar->addSeparator();
        toolBar->addAction(actionRight_allignment);
        toolBar->addAction(actionCenter_allignment);
        toolBar->addAction(actionLeft_allignment);
        toolBar->addAction(actionJustify);
        toolBar->addSeparator();
        toolBar->addAction(actionSottolinea);
        toolBar->addAction(actionGrassetto);
        toolBar->addAction(actionCorsivo);
        toolBar->addSeparator();
        toolBar->addAction(actionDiminuisci_rientro);
        toolBar->addAction(actionAumenta_rientro);
        toolBar->addSeparator();
        toolBar->addAction(actionTabella);
        toolBar->addAction(actionImmagine);
        toolBar->addSeparator();
        toolBar->addAction(actionTrova_e_sostituisci);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen->setText(QApplication::translate("MainWindow", "Apri..", nullptr));
        actionNew->setText(QApplication::translate("MainWindow", "Nuovo", nullptr));
        actionSave_as->setText(QApplication::translate("MainWindow", "Salva come..", nullptr));
        actionCopy->setText(QApplication::translate("MainWindow", "Copia", nullptr));
        actionCut->setText(QApplication::translate("MainWindow", "Taglia", nullptr));
        actionPaste->setText(QApplication::translate("MainWindow", "Incolla", nullptr));
        actionUndo->setText(QApplication::translate("MainWindow", "Indietro", nullptr));
        actionRedo->setText(QApplication::translate("MainWindow", "Avanti", nullptr));
        actionExit->setText(QApplication::translate("MainWindow", "Esci", nullptr));
        actionRight_allignment->setText(QApplication::translate("MainWindow", "A destra", nullptr));
        actionLeft_allignment->setText(QApplication::translate("MainWindow", "A sinistra", nullptr));
        actionCenter_allignment->setText(QApplication::translate("MainWindow", "Centrato", nullptr));
        actionJustify->setText(QApplication::translate("MainWindow", "Giustificato", nullptr));
        actionSelect_all->setText(QApplication::translate("MainWindow", "Seleziona tutto", nullptr));
        actionAumenta_rientro->setText(QApplication::translate("MainWindow", "Aumenta rientro", nullptr));
        actionDiminuisci_rientro->setText(QApplication::translate("MainWindow", "Diminuisci rientro", nullptr));
        actionSottolinea->setText(QApplication::translate("MainWindow", "Sottolinea", nullptr));
        actionGrassetto->setText(QApplication::translate("MainWindow", "Grassetto", nullptr));
        actionCorsivo->setText(QApplication::translate("MainWindow", "Corsivo", nullptr));
        actionTrova_e_sostituisci->setText(QApplication::translate("MainWindow", "Trova e sostituisci", nullptr));
        actionEsporta_PDF->setText(QApplication::translate("MainWindow", "Esporta PDF", nullptr));
        actionTabella->setText(QApplication::translate("MainWindow", "Tabella", nullptr));
        actionImmagine->setText(QApplication::translate("MainWindow", "Immagine", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Modifica", nullptr));
        menuParagraph->setTitle(QApplication::translate("MainWindow", "Formato", nullptr));
        menuAlignment->setTitle(QApplication::translate("MainWindow", "Allineamento", nullptr));
        menuTesto->setTitle(QApplication::translate("MainWindow", "Testo", nullptr));
        menuInserisci->setTitle(QApplication::translate("MainWindow", "Inserisci", nullptr));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
