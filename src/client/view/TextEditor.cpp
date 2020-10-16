#include "TextEditor.h"
#include "../../../cmake-build-debug/VirgiliumClient_autogen/include/ui_TextEditor.h"

#include <QComboBox>
#include <QFontComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QTextList>
#include <QToolButton>
#include <QColorDialog>
#include <QPrinter>
#include <utility>
#include <client/clientstuff.h>

TextEditor::TextEditor(QWidget *parent, ClientSocket *socket, const QString &fileName, User user) : QMainWindow(parent),
                                                                                                    ui(new Ui::TextEditor) {
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    this->setMinimumSize(900, 550);
    QRegExp tagExp("/");
    QStringList dataList = fileName.split(tagExp);
    this->setWindowTitle(dataList.at(1));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/Icons/v.png"), QSize(), QIcon::Normal, QIcon::On);
    this->setWindowIcon(icon);
    this->fileName = fileName;
    this->currentUser = std::move(user);

    ui->textEdit->setStyleSheet("QTextEdit { padding-left:150; padding-right:150;}");

    QTextCursor cursor(ui->textEdit->textCursor());
    QTextBlockFormat textBlockFormat = cursor.block().blockFormat();
    alignment = QString::number(textBlockFormat.alignment());
    indentation = QString::number(textBlockFormat.indent());

    /* header has an horizontal layout, in which the first item is the imageLabel with the image, and the second
    item is the internalLayoutHeader, that contains user name, menubar and toolbar */
    auto *header = new QWidget();
    QLayout *hBox = new QHBoxLayout();
    header->setLayout(hBox);
    auto *internalLayoutHeader = new QWidget();
    internalLayoutHeader->setLayout(new QVBoxLayout());

    auto *imageLabel = new QLabel();
    imageLabel->setPixmap(QPixmap(":/Icons/file.png"));
    imageLabel->setFixedWidth(50);
    imageLabel->setFixedHeight(50);
    imageLabel->setScaledContents(true);

    /* image and internalLayout are inserted in the header */
    header->layout()->addWidget(imageLabel);
    header->layout()->addWidget(internalLayoutHeader);

    /* Here, internalLayoutHeader is populated */
    internalLayoutHeader->layout()->setContentsMargins(0, 0, 0, 0);
    auto *buttonsLayout = new QWidget();
    QLayout *btnLayout = new QHBoxLayout();
    btnLayout->setAlignment(Qt::AlignLeft);
    buttonsLayout->setLayout(btnLayout);

    //QRegExp tagExp("/");
    //QStringList dataList = fileName.split(tagExp);
    //auto *userPage = new QPushButton(dataList[0]); //TODO forse possiamo pure togliere
    //userPage->setMaximumWidth(100);
    //userPage->setStyleSheet("background: #F0F0F0;");
    //buttonsLayout->layout()->addWidget(userPage);

    comboUsers = new QComboBox();
    comboUsers->setObjectName("comboUsers");
    comboUsers->setEditable(false);
    comboUsers->setFixedWidth(250);
    buttonsLayout->layout()->addWidget(comboUsers);

    internalLayoutHeader->layout()->addWidget(buttonsLayout);
    internalLayoutHeader->layout()->addWidget(ui->menubar);
    internalLayoutHeader->layout()->addWidget(ui->toolBar);
    ui->menubar->setStyleSheet("background: #F0F0F0;");

    addToolBar(ui->toolBar);
    internalLayoutHeader->layout()->addWidget(ui->toolBar);
    this->layout()->setMenuBar(header);

    /* Font size comboBox */
    ui->toolBar->addSeparator();
    drawFontSizeComboBox();

    /* Used to leave some space between widgets */
    auto *empty = new QWidget();
    empty->setFixedSize(5, 5);
    ui->toolBar->addWidget(empty);

    /* Font comboBox and color menu */
    drawFontComboBox();
    drawColorButton();

    /* client instance is created and connections for editor management are inserted  */
    this->client = new Crdt_editor(nullptr, socket, this->fileName);
    QObject::connect(this->client, &Crdt_editor::insert_into_window, this, &TextEditor::insert_text);
    QObject::connect(this->client, &Crdt_editor::remove_into_window, this, &TextEditor::delete_text);
    QObject::connect(this->client, &Crdt_editor::change_block_format, this, &TextEditor::changeBlockFormat);
    QObject::connect(this->client, &Crdt_editor::change_char_format, this, &TextEditor::changeCharFormat);
    QObject::connect(this->client, &Crdt_editor::change_cursor_position, this, &TextEditor::changeCursorPosition);
    QObject::connect(ui->textEdit->document(), SIGNAL(contentsChange(int, int, int)), this,
                     SLOT(change(int, int, int)));
    QObject::connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursorMoved()));
    QObject::connect(this->client, &Crdt_editor::load_response, this, &TextEditor::loadResponse);
    QObject::connect(this->client, &Crdt_editor::change_active_users, this, &TextEditor::changeActiveUser);

    this->loadRequest(this->fileName, this->currentUser);
}

TextEditor::~TextEditor() {
    this->client->deleteFromActive(this->currentUser, this->fileName);
    delete client;
    delete ui;
}

/* This function is used to load the current document from server and the list
 * of all users that are now modifying the document. */
void TextEditor::loadRequest(const QString &f, User user) {
    this->client->loadRequest(this->fileName, std::move(user));
}

/* This slot is used to get the response of the server. It contains the list
 * of symbols and the list of users. */
void TextEditor::loadResponse(_int code, const QVector<Symbol> &symbols, const QList<User> &users) {
    if (code == LOAD_RESPONSE) {
        for (const Symbol &symbol : symbols) {
            //spdlog::debug("{0}, {1}, {2}", symbol.getLetter().toStdString(), symbols.indexOf(symbol), symbol.getFont().font.toStdString());
            insertOneChar(symbols.indexOf(symbol), symbol.getLetter(), symbol.getFont(), symbol.getSiteId());
            insertOneChar(symbols.indexOf(symbol), symbol.getLetter(), symbol.getFont());
            changeBlockFormat(symbol.getFont().font, symbols.indexOf(symbol), symbols.indexOf(symbol) + 1);
        }
    }

    changeActiveUser(users);
}

/* This slot is used to change the actual active users */
void TextEditor::changeActiveUser(const QList<User> &users) {
    if (users.size() > this->activeUsers.size()) { //TODO spostare questa cosa nel model
        for (User user : users) {
            if (!this->activeUsers.contains(user)) {
                user.setAssignedColor(QColor(QRandomGenerator::global()->bounded(64, 192),
                                             QRandomGenerator::global()->bounded(64, 192),
                                             QRandomGenerator::global()->bounded(64, 192)));
                user.setLastCursorPos(0);
                this->activeUsers.push_back(user);
            }
        }
    } else if (users.size() < this->activeUsers.size()) {
        for (const User &user : this->activeUsers) {
            if (!users.contains(user)) {
                this->activeUsers.removeOne(user);
                if (user.getLastCursorPos() != 0)
                    changeBackground(user.getLastCursorPos(), Qt::white);
            }
        }
    }

    this->comboUsers->clear();
    auto *model = dynamic_cast< QStandardItemModel * >( comboUsers->model());
    for (int i = 0; i < this->activeUsers.size(); i++) {
        QString str = this->activeUsers.at(i).getFirstName() + " " + this->activeUsers.at(i).getLastName();
        comboUsers->addItem(str);
        comboUsers->setItemData(i, QBrush(this->activeUsers.at(i).getAssignedColor()), Qt::TextColorRole);
        auto *item = model->item(i);
        item->setSelectable(false);
    }
}

/* This function is used just to draw the font size comboBox, and within it the corresponding slot is called */
void TextEditor::drawFontSizeComboBox() {
    /* The comboBox is created */
    auto *comboSize = new QComboBox(ui->toolBar);
    comboSize->setObjectName("comboSize");
    comboSize->setEditable(true);

    /* The item are added from a standard list of different size */
    const QList<int> standardSizes = QFontDatabase::standardSizes();
    for (int size : standardSizes) {
        comboSize->addItem(QString::number(size));
    }

    /* The default value is set and the comboBox is connected to the corresponding slot */
    comboSize->setCurrentIndex(6);
    ui->toolBar->addWidget(comboSize);
    connect(comboSize, SIGNAL(currentIndexChanged(QString)), SLOT(changeFontSize(QString)));
}

void TextEditor::changeFontSize(const QString &selected) {
    ui->textEdit->setFontPointSize(selected.toInt());
}


/* This function is used just to draw the font comboBox, inside it a lambda function is used to perform the requested actions */
void TextEditor::drawFontComboBox() {
    /* The fontComboBox is created */
    myFont = new QFontComboBox;
    myFont->setEditable(false);
    ui->toolBar->addWidget(myFont);

    /* The comboBox is connected to a lambda function that change the font of the text */
    QObject::connect(myFont, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
                     [=](const QString &text) {
                         QTextCursor cursor(ui->textEdit->textCursor());
                         if (!cursor.hasSelection()) {
                             auto *qd = new QDialog;
                             qd->setWindowTitle("Change font");
                             QIcon icon;
                             icon.addFile(QString::fromUtf8(":/Icons/v.png"), QSize(), QIcon::Normal, QIcon::On);
                             qd->setWindowIcon(icon);
                             auto *vl = new QVBoxLayout;
                             qd->setLayout(vl);

                             auto label = new QLabel("Please, select the text that you want to change.");
                             auto *ok = new QPushButton("Ok");

                             vl->addWidget(label);
                             vl->addWidget(ok);

                             QObject::connect(ok, &QPushButton::clicked,
                                              [qd]() {
                                                  qd->close();
                                              });

                             qd->show();
                             //ui->textEdit->setFont(text); //TODO si potrebbe risolvere con QDialog
                         } else {
                             ui->textEdit->setCurrentFont(text);
                         }
                     });
}

/* This function is used just to draw the button of color text, and within it the corresponding slot is called */
void TextEditor::drawColorButton() {
    /* General widget is created */
    auto *qw = new QWidget();
    auto *vl = new QVBoxLayout();
    vl->setSpacing(2);
    qw->setLayout(vl);

    /* Here, the button is designed */
    const QSize btnSize = QSize(15, 15);
    auto *b = new QPushButton();
    b->setFixedSize(btnSize);
    QPixmap pixmap(":/Icons/color.png");
    QIcon ButtonIcon1(pixmap);
    b->setIcon(ButtonIcon1);
    b->setIconSize(btnSize);
    QString buttonStyle = "QPushButton{border:none;}";
    b->setStyleSheet(buttonStyle);

    /* A label is inserted above the button in order to show the selected color */
    textColorLabel = new QLabel();
    QColor color = ui->textEdit->textColor();
    QString colorS = color.name();
    QString labelStyle = "QLabel{background-color: " + colorS + "}";
    textColorLabel->setStyleSheet(labelStyle);
    textColorLabel->setFixedWidth(15);
    textColorLabel->setFixedHeight(4);

    /* Button and Label are inserted in the widget layout */
    vl->addWidget(b);
    vl->addWidget(textColorLabel);

    /* widget is inserted on the toolbar */
    ui->toolBar->addWidget(qw);

    /* The button is connected to the slot that open the QColorDialog */
    QObject::connect(b, &QToolButton::clicked, [this]() { TextEditor::changeColorSlot(); });
}

void TextEditor::changeColorSlot() {
    /* The selected color is taken */
    QColor color = QColorDialog::getColor(Qt::yellow, this);

    /* If color is valid, text and label are changed */
    if (color.isValid()) {
        ui->textEdit->setTextColor(color.name());
        QString labelStyle = "QLabel{background-color: " + color.name() + "}";
        textColorLabel->setStyleSheet(labelStyle);
    }
}

void TextEditor::on_actionExit_triggered() {
    this->close();
}

void TextEditor::on_actionCopy_triggered() {
    ui->textEdit->copy();
}

void TextEditor::on_actionPaste_triggered() {
    ui->textEdit->paste();
}

void TextEditor::on_actionCut_triggered() {
    ui->textEdit->cut();
}

void TextEditor::on_actionUndo_triggered() { //TODO fanno danno con il doppio client
    ui->textEdit->undo();
}

void TextEditor::on_actionRedo_triggered() { //TODO fanno danno con il doppio client
    ui->textEdit->redo();
}

void TextEditor::on_actionSelect_all_triggered() {
    ui->textEdit->selectAll();
}

void TextEditor::on_actionUnderline_triggered() {
    QTextCursor cursor = ui->textEdit->textCursor();
    if (cursor.hasSelection()) {
        cursor.setPosition(cursor.selectionStart() + 1);
        ui->textEdit->setFontUnderline(!cursor.charFormat().fontUnderline());
    } else {
        ui->textEdit->setFontUnderline(!ui->textEdit->fontUnderline());
    }
}

void TextEditor::on_actionBold_triggered() {
    QTextCursor cursor = ui->textEdit->textCursor();
    if (cursor.hasSelection()) {
        cursor.setPosition(cursor.selectionStart() + 1);
        ui->textEdit->setFontWeight(cursor.charFormat().fontWeight() == 50 ? 75 : 50);
    } else {
        ui->textEdit->setFontWeight(ui->textEdit->fontWeight() == 50 ? 75 : 50);
    }
}

void TextEditor::on_actionItalic_triggered() {
    QTextCursor cursor = ui->textEdit->textCursor();
    if (cursor.hasSelection()) {
        cursor.setPosition(cursor.selectionStart() + 1);
        ui->textEdit->setFontItalic(!cursor.charFormat().fontItalic());
    } else {
        ui->textEdit->setFontItalic(!ui->textEdit->fontItalic());
    }
}

void TextEditor::on_actionFind_and_replace_triggered() {
    /* A dialog is created in order to take find and replace value */
    auto *qd = new QDialog;
    qd->setWindowTitle("Find and replace");
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/Icons/v.png"), QSize(), QIcon::Normal, QIcon::On);
    qd->setWindowIcon(icon);
    auto *vl = new QVBoxLayout;
    qd->setLayout(vl);
    auto *labelFind = new QLabel("Find");
    auto *labelReplace = new QLabel("Replace");
    auto *ok = new QPushButton("Search");

    vl->addWidget(labelFind);
    vl->addWidget(this->lineFind);
    vl->addWidget(labelReplace);
    vl->addWidget(this->lineReplace);
    vl->addWidget(ok);

    /* Inside the lambda function all occurrences of "find" are replaced with "replace" and QLineEdit are cleared */
    QObject::connect(ok, &QPushButton::clicked,
                     [this]() {
                         this->find = this->lineFind->text().toUtf8().data();
                         this->replace = this->lineReplace->text().toUtf8().data();

                         ui->textEdit->moveCursor(QTextCursor::Start);
                         while (ui->textEdit->find(find)) {
                             ui->textEdit->textCursor().insertText(replace);
                         }
                         this->lineFind->clear();
                         this->lineReplace->clear();
                     });

    qd->show();
}

void TextEditor::on_actionIncrease_indent_triggered() {
    changeIndentSpacing(1);
    QTextCursor cursor(ui->textEdit->textCursor());
    if (cursor.document()->isEmpty())
        sendBlockFormatChange();
}

void TextEditor::on_actionDecrease_indent_triggered() {
    changeIndentSpacing(-1);
    QTextCursor cursor(ui->textEdit->textCursor());
    if (cursor.document()->isEmpty())
        sendBlockFormatChange();
}

void TextEditor::changeIndentSpacing(int num) {
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.beginEditBlock();
    if (cursor.currentList()) {
        QTextListFormat listFmt = cursor.currentList()->format();
        QTextCursor above(cursor);
        above.movePosition(QTextCursor::Up);
        if (above.currentList() && listFmt.indent() + num == above.currentList()->format().indent()) {
            above.currentList()->add(cursor.block());
        } else {
            listFmt.setIndent(listFmt.indent() + num);
            cursor.createList(listFmt);
        }
    } else {
        QTextBlockFormat blockFmt = cursor.blockFormat();
        blockFmt.setIndent(blockFmt.indent() + num);
        cursor.setBlockFormat(blockFmt);
    }
    cursor.endEditBlock();
}

void TextEditor::on_actionExport_PDF_triggered() {
    QFileDialog fileDialog(this, tr("Export PDF"));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setMimeTypeFilters(QStringList("application/pdf"));
    fileDialog.setDefaultSuffix("pdf");
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    QString file_name = fileDialog.selectedFiles().first();
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(file_name);
    ui->textEdit->document()->print(&printer);
    statusBar()->showMessage(tr("Exported \"%1\"")
                                     .arg(QDir::toNativeSeparators(file_name)));
}

void TextEditor::on_actionRight_alignment_triggered() {
    if (alignment != "2")
        ui->textEdit->setAlignment(Qt::AlignRight);
    QTextCursor cursor(ui->textEdit->textCursor());
    if (cursor.document()->isEmpty())
        sendBlockFormatChange();
}

void TextEditor::on_actionLeft_alignment_triggered() {
    if (alignment != "1")
        ui->textEdit->setAlignment(Qt::AlignLeft);
    QTextCursor cursor(ui->textEdit->textCursor());
    if (cursor.document()->isEmpty())
        sendBlockFormatChange();
}

void TextEditor::on_actionCenter_alignment_triggered() {
    if (alignment != "132")
        ui->textEdit->setAlignment(Qt::AlignCenter);
    QTextCursor cursor(ui->textEdit->textCursor());
    if (cursor.document()->isEmpty())
        sendBlockFormatChange();
}

void TextEditor::on_actionJustify_triggered() {
    if (alignment != "8")
        ui->textEdit->setAlignment(Qt::AlignJustify);
    QTextCursor cursor(ui->textEdit->textCursor());
    if (cursor.document()->isEmpty())
        sendBlockFormatChange();
}


/* This slot is called when change_cursor_position signal is emitted, so it is invoked every time
 * someone else change the position of the cursor. Here, the user that performed the action is
 * found inside the users QVector and by considering data inside the User structure the new
 * position of the cursor is shown inside the other client editors. */
void TextEditor::changeCursorPosition(_int position, _int siteId) {
    /* The user that perform the action is searched */
    QTextCursor cursor(ui->textEdit->textCursor());
    if (position >= cursor.document()->characterCount())
        position = cursor.document()->characterCount() - 1;
    User u;
    for (User &user : this->activeUsers) {
        if (user.getSiteId() == siteId) {
            u = user;
            user.setLastCursorPos(position);
        }
    }

    /* If the last position of the cursor was 0, there wasn't any cursor show on the editor of the
     * other clients, so it is not necessary to delete the previous cursor */
    if (u.getLastCursorPos() != 0 && u.getLastCursorPos() <= cursor.document()->characterCount() - 1)
        changeBackground(u.getLastCursorPos(), Qt::white);

    /* When the previous cursor position is deleted, the new one is shown on the editor if the new
     * position is different than 0. */
    if (position != 0)
        changeBackground(position, u.getAssignedColor());
}

/* This function is used to change the background color by passing the position of the char that
 * must be changed and the color that must be applied. */
void TextEditor::changeBackground(_int position, const QColor &color) {
    ui->textEdit->document()->blockSignals(true);
    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.setPosition(position, QTextCursor::MoveAnchor);
    QTextCharFormat textCharFormat = cursor.charFormat();
    textCharFormat.setBackground(color);

    cursor.setPosition(position - 1, QTextCursor::MoveAnchor);
    cursor.setPosition(position, QTextCursor::KeepAnchor);
    cursor.setCharFormat(textCharFormat);
    ui->textEdit->document()->blockSignals(false);
}

/* This slot is called when insert_into_window signal is emitted, it has to insert the new char
 * inside the editor of the other clients. In fact, it accepts the char, the position where
 * it must be inserted and the font of the char. */
void TextEditor::insert_text(_int pos, const QString &character, const Symbol::CharFormat &font, _int siteId) {
    insertOneChar(pos, character, font);
    this->changeCursorPosition(pos + 1, siteId);
}

void TextEditor::insertOneChar(_int pos, const QString &character, const Symbol::CharFormat &font) {
    QTextCursor cursor(ui->textEdit->textCursor());
    int originalPosition = cursor.position();
    cursor.setPosition(pos);
    QTextCharFormat textCharFormat = ui->textEdit->currentCharFormat();

    /* It is used to retrieve all information inside the font value, font family, size, weight,
     * italic, underline, background and foreground color.*/
    QRegExp tagExp("/");
    QStringList firstList = font.font.split(tagExp);

    QRegExp tagExp2(",");
    QStringList fontList = font.font.split(tagExp2);
    QFont insertedFont;
    insertedFont.setFamily(fontList.at(0));
    insertedFont.setPointSize(fontList.at(1).toInt());
    insertedFont.setWeight(fontList.at(4).toInt());
    insertedFont.setItalic(!(fontList.at(5) == "0"));
    insertedFont.setUnderline(!(fontList.at(6) == "0"));

    textCharFormat.setFont(insertedFont);
    textCharFormat.setForeground(font.foreground);

    /* Here, there is the actual change of the char. */
    ui->textEdit->document()->blockSignals(true);
    cursor.insertText(character, textCharFormat);
    cursor.setPosition(originalPosition);
    ui->textEdit->document()->blockSignals(false);
}

/* This slot is called when remove_from_window signal is emitted, it has to delete the char
 * from the editor of the other clients. In fact, it accepts the position of the char that
 * must be deleted. */
void TextEditor::delete_text(_int pos, _int siteId) {
    /* The cursor is moved in the position where there is the char that must be deleted. */
    QTextCursor cursor(ui->textEdit->textCursor());
    int originalPosition = cursor.position();
    cursor.movePosition(QTextCursor::End);
    int maxPos = cursor.position();
    cursor.setPosition(pos);

    /* The char is deleted. */
    ui->textEdit->document()->blockSignals(true);
    cursor.deleteChar();
    ui->textEdit->document()->blockSignals(false);

    /* The cursor is moved back to the previous position. */
    if (originalPosition == maxPos)
        cursor.setPosition(originalPosition - 1);
    else
        cursor.setPosition(originalPosition);

    this->changeCursorPosition(pos, siteId);
}

void TextEditor::changeBlockFormat(const QString &font, _int startPos, _int finalPos) {
    QRegExp tagExp("/");
    QStringList firstList = font.split(tagExp);

    ui->textEdit->document()->blockSignals(true);
    QTextCursor cursor(ui->textEdit->textCursor());
    if (startPos != -1 && finalPos != -1) {
        cursor.setPosition(startPos, QTextCursor::MoveAnchor);
        cursor.setPosition(finalPos - 1, QTextCursor::KeepAnchor);
    }
    QTextBlockFormat textBlockFormat = cursor.block().blockFormat();
    switch (firstList.at(1).toInt()) {
        case 1:
            textBlockFormat.setAlignment(Qt::AlignLeft);
            if (alignment != "1")
                ui->textEdit->setAlignment(Qt::AlignLeft);
            break;
        case 2:
            textBlockFormat.setAlignment(Qt::AlignRight);
            if (alignment != "2")
                ui->textEdit->setAlignment(Qt::AlignRight);
            break;
        case 8:
            textBlockFormat.setAlignment(Qt::AlignJustify);
            if (alignment != "8")
                ui->textEdit->setAlignment(Qt::AlignJustify);
            break;
        case 132:
            textBlockFormat.setAlignment(Qt::AlignHCenter);
            if (alignment != "132")
                ui->textEdit->setAlignment(Qt::AlignCenter);
            break;
    }
    textBlockFormat.setIndent(firstList.at(2).toInt());
    cursor.setBlockFormat(textBlockFormat);

    alignment = QString::number(textBlockFormat.alignment());
    indentation = QString::number(textBlockFormat.indent());
    ui->textEdit->document()->blockSignals(false);
}

void TextEditor::changeCharFormat(_int pos, const Symbol::CharFormat &charData) {
    ui->textEdit->document()->blockSignals(true);
    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.setPosition(pos, QTextCursor::MoveAnchor);
    QTextCharFormat textCharFormat = cursor.charFormat();

    QRegExp tagExp2(",");
    QStringList fontList = charData.font.split(tagExp2);
    QFont insertedFont;
    insertedFont.setFamily(fontList.at(0));
    insertedFont.setPointSize(fontList.at(1).toInt());
    insertedFont.setWeight(fontList.at(4).toInt());
    insertedFont.setItalic(!(fontList.at(5) == "0"));
    insertedFont.setUnderline(!(fontList.at(6) == "0"));

    textCharFormat.setFont(insertedFont);
    textCharFormat.setForeground(charData.foreground);

    cursor.setPosition(pos, QTextCursor::MoveAnchor);
    cursor.setPosition(pos + 1, QTextCursor::KeepAnchor);
    cursor.setCharFormat(textCharFormat);
    ui->textEdit->document()->blockSignals(false);
}

/* This slot is called every time that the cursorPositionChanged() signal is emitted. It is only used
 * to send the new position of the cursor of my editor to the other clients. */
void TextEditor::cursorMoved() {
    QTextCursor cursor(ui->textEdit->textCursor());
    this->client->changeCursor(cursor.position());
}

/* This slot is called every time that the contentsChange(int, int, int) signal is emitted. The signal is
 * emitted when one or more chars are inserted or deleted. So, this function is used to get all
 * necessary information and send it to the server. */
void TextEditor::change(int pos, int del, int add) {
    /* Here, the format of the char is taken. */
    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.setPosition(cursor.selectionEnd(), QTextCursor::MoveAnchor);
    QTextCharFormat textCharFormat = cursor.charFormat();
    QTextBlockFormat textBlockFormat = cursor.block().blockFormat();

    QString result = textCharFormat.font().toString() + "/"
                     + QString::number(textBlockFormat.alignment()) + "/"
                     + QString::number(textBlockFormat.indent());

    Symbol::CharFormat charData;
    charData.foreground = textCharFormat.foreground().color();
    charData.font = result;

    if (add != 0 && del == 0) { /* insert chars */

        QString added = ui->textEdit->toPlainText().mid(pos, add);
        if (add == 1) {
            this->client->localInsert(pos, added, charData);
            changeBackground(pos + 1, Qt::white);
        } else {
            multipleInsert(pos, added);
        }

    } else if (add == 0 && del != 0) { /* delete chars */

        if (del == 1)
            this->client->localErase(pos);
        else
            multipleErase(pos, del);

    } else { /* Some chars are deleted and some other else are inserted. */
        ui->textEdit->undo();
        QString removed = ui->textEdit->toPlainText().mid(pos, add);
        ui->textEdit->redo();
        QString added = ui->textEdit->toPlainText().mid(pos, add);
        if (add == del) {
            /* The for loop is used to check if some chars are changed or if the slot is called only because
             * the format of one or more chars is changed. */
            bool equal = true;
            for (int i = 0; i < added.size(); i++) {
                if (added[i] != removed[i]) {
                    equal = false;
                    break;
                }
            }

            if (equal) { /* If chars are equal, it means that the format is changed. */
                /* Check if alignment or indentation are changed. */
                if (alignment == QString::number(textBlockFormat.alignment()) &&
                    indentation == QString::number(textBlockFormat.indent())) {
                    /*multipleErase(pos, removed.size());
                    multipleInsert(pos, added);*/
                    multipleUpdate(pos, removed.size(), charData);
                } else {
                    alignment = QString::number(textBlockFormat.alignment());
                    indentation = QString::number(textBlockFormat.indent());

                    if (pos + add < cursor.document()->characterCount())
                        this->client->changeBlockFormat(charData, pos, pos + add);
                    else
                        this->client->changeBlockFormat(charData, pos, pos + add - 2);
                }
                return;
            }
        }

        /* If we arrive here, it means that some chars must be deleted and some other else
         * must be inserted. */
        int originalPos = cursor.position();
        cursor.movePosition(QTextCursor::End);
        int maxPos = cursor.position();
        cursor.setPosition(originalPos);
        if (del == maxPos + 1) {
            for (int i = pos + del - 2; i >= pos; i--) {
                this->client->localErase(i);
            }
            multipleInsert(pos, added);
            return;
        }
        multipleErase(pos, removed.size());
        multipleInsert(pos, added);
    }
}

/* This function is invoked when it is necessary to add more than one char. */
void TextEditor::multipleInsert(int pos, const QString &added) {
    for (QString c : added) {
        QTextCursor cursor(ui->textEdit->textCursor());
        cursor.setPosition(pos + 1, QTextCursor::MoveAnchor);
        QTextCharFormat textCharFormat = cursor.charFormat();
        QTextBlockFormat textBlockFormat = cursor.block().blockFormat();

        QString result = textCharFormat.font().toString() + "/"
                         + QString::number(textBlockFormat.alignment()) + "/"
                         + QString::number(textBlockFormat.indent());

        Symbol::CharFormat charData;
        charData.foreground = textCharFormat.foreground().color();
        charData.font = result;

        cursor.setPosition(pos, QTextCursor::MoveAnchor);
        cursor.setPosition(pos + 1, QTextCursor::KeepAnchor);
        textCharFormat.setBackground(Qt::white);
        cursor.setCharFormat(textCharFormat);

        this->client->localInsert(pos, c, charData);
        pos++;
    }
}

/* This function is invoked when it is necessary to remove more than one char. */
void TextEditor::multipleErase(int pos, int del) {
    for (int i = pos + del - 1; i >= pos; i--) {
        this->client->localErase(i);
    }
}

void TextEditor::sendBlockFormatChange() {
    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.setPosition(cursor.selectionEnd(), QTextCursor::MoveAnchor);
    QTextCharFormat textCharFormat = cursor.charFormat();
    QTextBlockFormat textBlockFormat = cursor.block().blockFormat();

    QString result = textCharFormat.font().toString() + "/"
                     + QString::number(textBlockFormat.alignment()) + "/"
                     + QString::number(textBlockFormat.indent());

    Symbol::CharFormat charData;
    charData.foreground = textCharFormat.foreground().color();
    charData.font = result;

    this->client->changeBlockFormat(charData, -1, -1);
}

void TextEditor::multipleUpdate(_int pos, _int size, const Symbol::CharFormat &charData) {
    for (int i = 0; i < size; i++) {
        this->client->localUpdate(pos, charData);
        pos++;
    }
}
