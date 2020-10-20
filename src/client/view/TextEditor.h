#ifndef VIRGILIUM_TEXTEDITOR_H
#define VIRGILIUM_TEXTEDITOR_H

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
#include "PersonalPage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TextEditor; }
QT_END_NAMESPACE
class TextEditor : public QMainWindow {
Q_OBJECT

public:
    TextEditor(QWidget *parent, ClientSocket *socket, const QString &fileName, User user, PersonalPage *personalPage);

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

    /*
     * This function is used to draw the font comboBox, inside it a lambda function is used to perform the requested actions.
     */
    void drawFontComboBox();

    /*
     * This function is used just to draw the font size comboBox, and within it the corresponding slot is called.
     */
    void drawFontSizeComboBox();

    /*
     * this function is used by increaseIndentation and decreaseIndentation
     * to actually change the indentation of the text editor.
     */
    void changeIndentSpacing(int num);

    /*
     * This function is used just to draw the button of color text, and within it the corresponding slot is called.
     */
    void drawColorButton();

    /*
     * This function is invoked when it is necessary to add more than one char.
     */
    void multipleInsert(int pos, const QString &added);

    /*
     * This function is invoked when it is necessary to remove more than one char.
     */
    void multipleErase(int pos, int del);

    /*
     * This function is used to change the background color by passing the position of the char that
     * must be changed and the color that must be applied.
     */
    void changeBackground(_int position, const QColor &color);

    /*
     * This function is used to load the current document from server and the list
     * of all users that are now modifying the document.
     */
    void loadRequest(const QString &fileName, User user);

    /*
     * This function is called by insert_text and loadResponse methods in order to
     * insert a new symbol inside the document.
     */
    void insertOneChar(_int pos, const QString &character, const Symbol::CharFormat &font);

    /*
     * This method is used when the current user change more that one char format.
     */
    void multipleUpdate(_int pos, _int size);

    /*
     * This method is used to inform other clients in that case when there are not
     * symbols inside the document.
     */
    void sendBlockFormatChange();

private slots:

    /* close the editor. */
    void on_actionExit_triggered();

    /* copy operation. */
    void on_actionCopy_triggered();

    /* paste operation. */
    void on_actionPaste_triggered();

    /* cut operation. */
    void on_actionCut_triggered();

    /* select all operation. */
    void on_actionSelect_all_triggered();

    /* underline operation. */
    void on_actionUnderline_triggered();

    /* bold operation. */
    void on_actionBold_triggered();

    /* italic operation. */
    void on_actionItalic_triggered();

    /*
     * This slot is used to open the find and replace dialog, inside it a lambda function is
     * used to to perform the replace operation.
     */
    void on_actionFind_and_replace_triggered();

    /* increase indentation operation. */
    void on_actionIncrease_indent_triggered();

    /* decrease indentation operation. */
    void on_actionDecrease_indent_triggered();

    /*
     * This slot is used to create a PDF file with the content of the document.
     */
    void on_actionExport_PDF_triggered();

    /* right alignment operation. */
    void on_actionRight_alignment_triggered();

    /* left alignment operation. */
    void on_actionLeft_alignment_triggered();

    /* center alignment operation. */
    void on_actionCenter_alignment_triggered();

    /* justify alignment operation. */
    void on_actionJustify_triggered();

    /*
     * This is the slot called to actually change the font size.
     */
    void changeFontSize(const QString &selected);

    /*
     * This slot actually change the text color.
     */
    void changeColorSlot();

    /*
     * This slot is called when insert_into_window signal is emitted, it has to insert the new char
     * inside the editor of the other clients. In fact, it accepts the char, the position where
     * it must be inserted and the font of the char.
     */
    void insert_text(_int pos, const QString &character, const Symbol::CharFormat &font, _int siteId);

    /*
     * This slot is called when remove_from_window signal is emitted, it has to delete the char
     * from the editor of the other clients. In fact, it accepts the position of the char that
     * must be deleted.
     */
    void delete_text(_int pos, _int siteId);

    /*
     * This slot is called every time that the contentsChange(int, int, int) signal is emitted. The signal is
     * emitted when one or more chars are inserted or deleted. So, this function is used to get all
     * necessary information and send it to the server.
     */
    void change(int pos, int del, int add);

    /*
     * This slot is called every time that the cursorPositionChanged() signal is emitted. It is only used
     * to send the new position of the cursor of my editor to the other clients.
     */
    void cursorMoved();

    /*
     * This slot is called when change_cursor_position signal is emitted, so it is invoked every time
     * someone else change the position of the cursor. Here, the user that performed the action is
     * found inside the users QVector and by considering data inside the User structure the new
     * position of the cursor is shown inside the other client editors.
     */
    void changeCursorPosition(_int position, _int siteId);

    /*
     * This slot is used to get the response of the server. It contains the list
     * of symbols and the list of users.
     */
    void loadResponse(_int code, const QVector<Symbol> &symbols, const QList<User> &users);

    /*
     * This slot is used to change the actual active users.
     */
    void changeActiveUser(const QList<User> &activeUsers);

    /*
     * This slot is invoked when alignment or indentation is modify by someone else, so
     * it is necessary to update also the current editor.
     */
    void changeBlockFormat(const QString &font);

    /*
     * This slot is invoked when the char format is modify by someone else, so
     * it is necessary to update also the current editor.
     */
    void changeCharFormat(_int pos, const Symbol::CharFormat &charData);

    /*
     * This slot is called when the personal page is closed.
     */
    void closeEditor();

};

#endif //VIRGILIUM_TEXTEDITOR_H
