#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <client/clientstuff.h>
#include "common/CRDT/Crdt_editor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TextEditor; }
QT_END_NAMESPACE
class TextEditor : public QMainWindow {
Q_OBJECT

public:
    TextEditor(QWidget *parent, ClientSocket *socket, const QString &fileName, User user);

    ~TextEditor();

private:
    Ui::TextEditor *ui{};
    QString find;
    QString replace;
    QLineEdit *lineFind = new QLineEdit;
    QLineEdit *lineReplace = new QLineEdit;
    QLabel *textColorLabel{};
    Crdt_editor *client;
    QString alignment;
    QString indentation;
    QList<User> activeUsers;
    QString fileName;
    User currentUser;
    QComboBox *comboUsers;
    QFontComboBox *myFont;

    void drawFontComboBox();

    void drawFontSizeComboBox();

    void changeIndentSpacing(int num);

    void drawColorButton();

    void multipleInsert(int pos, const QString &added);

    void multipleErase(int pos, int del);

    void changeBackground(_int position, const QColor &color);

    void loadRequest(const QString &fileName, User user);

    void insertOneChar(_int pos, const QString &character, const Symbol::CharFormat &font);

    void multipleUpdate(_int pos, _int size);

    void sendBlockFormatChange();

private slots:

    void on_actionExit_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_actionSelect_all_triggered();

    void on_actionUnderline_triggered();

    void on_actionBold_triggered();

    void on_actionItalic_triggered();

    void on_actionFind_and_replace_triggered();

    void on_actionIncrease_indent_triggered();

    void on_actionDecrease_indent_triggered();

    void on_actionExport_PDF_triggered();

    void on_actionRight_alignment_triggered();

    void on_actionLeft_alignment_triggered();

    void on_actionCenter_alignment_triggered();

    void on_actionJustify_triggered();

    void changeFontSize(const QString &selected);

    void changeColorSlot();

    void insert_text(_int pos, const QString &character, const Symbol::CharFormat &font, _int siteId);

    void delete_text(_int pos, _int siteId);

    void change(int pos, int del, int add);

    void cursorMoved();

    void changeCursorPosition(_int position, _int siteId);

    void loadResponse(_int code, const QVector<Symbol> &symbols, const QList<User> &users);

    void changeActiveUser(const QList<User> &activeUsers);

    void changeBlockFormat(const QString &font, _int startPos, _int finalPos);

    void changeCharFormat(_int pos, const Symbol::CharFormat &charData);

};

#endif //TEXTEDITOR_H
