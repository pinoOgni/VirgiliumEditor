#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include "virgilium_client.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void drawListButton();
    void drawFontComboBox();
    void drawFontSizeComboBox();
    void modificaIndentazione(int num);
    void drawColorButton();
    void drawHighlighterButton();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_actionExit_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionRight_allignment_triggered();

    void on_actionLeft_allignment_triggered();

    void on_actionCenter_allignment_triggered();

    void on_actionJustify_triggered();

    void on_actionSelect_all_triggered();

    void on_actionSottolinea_triggered();

    void on_actionGrassetto_triggered();

    void on_actionCorsivo_triggered();

    void on_actionTrova_e_sostituisci_triggered();

    void on_actionAumenta_rientro_triggered();

    void on_actionDiminuisci_rientro_triggered();

    void on_actionEsporta_PDF_triggered();

    void changeFontSize(const QString& selected);

    void listSlot(int value);

    void on_actionTabella_triggered();

    void on_actionImmagine_triggered();

    void changeColorSlot();

    void highlightSlot(int color);

    void on_textEdit_textChanged();
    void change_text(_int pos, QString character);

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    QDialog *qd = new QDialog;
    QString find;
    QString replace;
    QLineEdit *lineT = new QLineEdit;
    QLineEdit *lineS = new QLineEdit;
    int row;
    int column;
    int columnWidth;
    QSpinBox *lineR = new QSpinBox;
    QSpinBox *lineC = new QSpinBox;
    QSpinBox *cWidth = new QSpinBox;
    QComboBox *comboBox = new QComboBox;
    QLabel *textColorLabel;
    QString textValue;
    QString newTextValue;
    virgilium_client *client;
};
#endif // MAINWINDOW_H
