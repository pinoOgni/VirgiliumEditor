#include "TextEditor.h"
#include "ui_TextEditor.h"

#include <QComboBox>
#include <QFontComboBox>
#include <QLineEdit>
#include <QLabel>
#include <iostream>
#include <QTextList>
#include <QToolButton>
#include <QColorDialog>
#include <QPrinter>

TextEditor::TextEditor(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::TextEditor)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    this->setMinimumSize(900, 550);
    this->setWindowTitle("Virgilium");

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

    auto *userPage = new QPushButton("User Name"); //it should contains the name of the authenticated user
    userPage->setMaximumWidth(100);
    userPage->setStyleSheet("background: #F0F0F0;");
    buttonsLayout->layout()->addWidget(userPage);

    auto *comboUsers = new QComboBox();
    comboUsers->setObjectName("comboUsers");
    comboUsers->setEditable(false);
    const QList<QString> userList = {"Actual active users", "Simone D'Amilo", "Ajeje Brazov"}; //take list of users from database
    auto *model = dynamic_cast< QStandardItemModel * >( comboUsers->model());
    for (const QString &user : userList) {
        comboUsers->addItem(user);
        auto *item = model->item(userList.indexOf(user), 0);
        item->setSelectable(false);
    }
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
    //drawHighlighterButton();

    /* client instance is created and connections for editor management are inserted  */
    this->client = new virgilium_client(nullptr, "127.0.0.1", 9999);
    QObject::connect(this->client, &virgilium_client::insert_into_window, this, &TextEditor::insert_text);
    QObject::connect(this->client, &virgilium_client::remove_into_window, this, &TextEditor::delete_text);
    QObject::connect(this->client, &virgilium_client::change_cursor_position, this, &TextEditor::changeCursorPosition);
    QObject::connect(ui->textEdit->document(), SIGNAL(contentsChange(int, int, int)), this,
                     SLOT(change(int, int, int)));
    QObject::connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this,
                     SLOT(cursorMoved()));

    //l'intera struttura deve essere mantenuta dal server.
    User user;
    user.name = "Simone"; //preso dal db
    user.surname = "DAmilo"; //preso dal db
    user.siteId = 21; //assegnato dal sistema
    user.assignedColor = Qt::red; //assegnato in modo casuale
    user.lastCursorPos = 0; //inizialmente sempre a 0
    User user2;
    user2.name = "Simone";
    user2.surname = "DAmilo";
    user2.siteId = 22;
    user2.assignedColor = Qt::yellow;
    user2.lastCursorPos = 0;
    User user3;
    user3.name = "Simone";
    user3.surname = "DAmilo";
    user3.siteId = 6;
    user3.assignedColor = Qt::blue;
    user3.lastCursorPos = 0;
    users = {user, user2, user3};
}

TextEditor::~TextEditor() {
    delete ui;
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
    auto *myFont = new QFontComboBox;
    myFont->setEditable(false);
    ui->toolBar->addWidget(myFont);

    /* The comboBox is connected to a lambda function that change the font of the text */
    QObject::connect(myFont, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
                     [=](const QString &text) {
                         QTextCursor cursor(ui->textEdit->textCursor());
                         if (!cursor.hasSelection()) {
                             ui->textEdit->setFont(text); //TODO si potrebbe risolvere con QDialog
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


/* This function is used just to draw the button of highlighter, and within it the corresponding slot is called */
/*void MainWindow::drawHighlighterButton() {
    *//* A grid layout is set to the menu *//*
    auto *menu = new QMenu(this);
    auto *gl = new QGridLayout();
    menu->setLayout(gl);

    const QSize btnSize = QSize(15, 15);        *//* size of each button *//*

    *//* In the for I create, insert and connect the 10 buttons *//*
    QList<QString> colors = {"white", "green", "cyan", "blue", "red", "magenta", "gray", "darkBlue", "yellow", "black"};
    for (int i = 0; i < 10; i++) {
        auto *b = new QPushButton();
        b->setFixedSize(btnSize);
        QString buttonStyle = "QPushButton{background-color:" + colors[i] + ";}";
        b->setStyleSheet(buttonStyle);
        if (i < 5) {
            gl->addWidget(b, 0, i, 1, 1);
        } else {
            gl->addWidget(b, 1, i - 5, 1, 1);
        }
        QObject::connect(b, &QPushButton::clicked, [this, i]() { MainWindow::highlightSlot(i + 1); });
    }

    *//* Button on the toolbar is set *//*
    auto *act0 = new QAction(this);
    QIcon icon0;
    icon0.addFile(QString::fromUtf8(":/Icons/highlighter.png"), QSize(), QIcon::Normal, QIcon::On);
    act0->setIcon(icon0);

    *//* Menu is inserted in the button, which is added to the toolbar *//*
    auto *toolButton = new QToolButton(this);
    toolButton->setMenu(menu);
    toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    toolButton->setDefaultAction(act0);
    QObject::connect(toolButton, &QToolButton::clicked, [this]() { MainWindow::highlightSlot(1); });
    ui->toolBar->addWidget(toolButton);
}*/

/* QUESTA DEVE ESSERE ELIMINATA SE SI LASCIA IL CURSORE CON L'EVIDENZIATORE */
/*void MainWindow::highlightSlot(int color) {
    QTextCharFormat fmt = ui->textEdit->currentCharFormat();
    *//* Depending on the value passed from drawHighlighterButton function, fmt is set *//*
    switch (color) {
        case 1:
            fmt.setBackground(Qt::white);
            break;
        case 2:
            fmt.setBackground(Qt::green);
            break;
        case 3:
            fmt.setBackground(Qt::cyan);
            break;
        case 4:
            fmt.setBackground(Qt::blue);
            break;
        case 5:
            fmt.setBackground(Qt::red);
            break;
        case 6:
            fmt.setBackground(Qt::magenta);
            break;
        case 7:
            fmt.setBackground(Qt::gray);
            break;
        case 8:
            fmt.setBackground(Qt::darkBlue);
            break;
        case 9:
            fmt.setBackground(Qt::yellow);
            break;
        case 10:
            fmt.setBackground(Qt::black);
            break;
        default:
            break;
    }

    *//* Now, that the fmt variable is ready, text is changed by using cursor and hasSelection method that returns
    true is some text is selected *//*
    QTextCursor cursor(ui->textEdit->textCursor());
    if (cursor.hasSelection()) {
        cursor.setCharFormat(fmt);
    }
}*/

void TextEditor::on_actionExit_triggered() {
    QApplication::quit();
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

void TextEditor::on_actionUndo_triggered() {
    ui->textEdit->undo();
}

void TextEditor::on_actionRedo_triggered() {
    ui->textEdit->redo();
}

void TextEditor::on_actionSelect_all_triggered() {
    ui->textEdit->selectAll();
}

void TextEditor::on_actionUnderline_triggered() {
    ui->textEdit->setFontUnderline(!ui->textEdit->fontUnderline());
}

void TextEditor::on_actionBold_triggered() {
    if (ui->textEdit->fontWeight() == 50) {
        ui->textEdit->setFontWeight(75);
    } else {
        ui->textEdit->setFontWeight(50);
    }
}

void TextEditor::on_actionItalic_triggered() {
    ui->textEdit->setFontItalic(!ui->textEdit->fontItalic());
}

void TextEditor::on_actionFind_and_replace_triggered() {
    /* A dialog is created in order to take find and replace value */
    auto *qd = new QDialog;
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
}

void TextEditor::on_actionDecrease_indent_triggered() {
    changeIndentSpacing(-1);
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
    qDebug() << "on_actionExport_PDF_triggered";
    QFileDialog fileDialog(this, tr("Export PDF"));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setMimeTypeFilters(QStringList("application/pdf"));
    fileDialog.setDefaultSuffix("pdf");
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    QString fileName = fileDialog.selectedFiles().first();
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    ui->textEdit->document()->print(&printer);
    statusBar()->showMessage(tr("Exported \"%1\"")
                                     .arg(QDir::toNativeSeparators(fileName)));
}

void TextEditor::on_actionRight_alignment_triggered() {
    if(alignment != "2")
        ui->textEdit->setAlignment(Qt::AlignRight);
}

void TextEditor::on_actionLeft_alignment_triggered() {
    if(alignment != "1")
        ui->textEdit->setAlignment(Qt::AlignLeft);
}

void TextEditor::on_actionCenter_alignment_triggered() {
    if(alignment != "132")
        ui->textEdit->setAlignment(Qt::AlignCenter);
}

void TextEditor::on_actionJustify_triggered() {
    if(alignment != "8")
        ui->textEdit->setAlignment(Qt::AlignJustify);
}


/* This slot is called when change_cursor_position signal is emitted, so it is invoked every time
 * someone else change the position of the cursor. Here, the user that performed the action is
 * found inside the users QVector and by considering data inside the User structure the new
 * position of the cursor is shown inside the other client editors. */
void TextEditor::changeCursorPosition(_int position, _int siteId) {
    /* The user that perform the action is searched */
    User currentUser;
    for (User &user : users) {
        if (user.siteId == siteId) {
            currentUser = user;
            user.lastCursorPos = position;
        }
    }

    ui->textEdit->document()->blockSignals(true);

    /* If the last position of the cursor was 0, there wasn't any cursor show on the editor of the
     * other clients, so it is not necessary to delete the previous cursor */
    if (currentUser.lastCursorPos != 0)
        changeBackground(currentUser.lastCursorPos, Qt::white);

    /* When the previous cursor position is deleted, the new one is shown on the editor if the new
     * position is different than 0. */
    if (position != 0)
        changeBackground(position, currentUser.assignedColor);

    ui->textEdit->document()->blockSignals(false);
}

/* This function is used to change the background color by passing the position of the char that
 * must be changed and the color that must be applied. */
void TextEditor::changeBackground(_int position, const QColor &color) {
    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.setPosition(position, QTextCursor::MoveAnchor);
    QTextCharFormat textCharFormat = cursor.charFormat();
    textCharFormat.setBackground(color);

    cursor.setPosition(position - 1, QTextCursor::MoveAnchor);
    cursor.setPosition(position, QTextCursor::KeepAnchor);
    cursor.setCharFormat(textCharFormat);
}

/* This slot is called when insert_into_window signal is emitted, it has to insert the new char
 * inside the editor of the other clients. In fact, it accepts the char, the position where
 * it must be inserted and the font of the char. */
void TextEditor::insert_text(_int pos, const QString &character, const Symbol::CharFormat &font) {
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
    //textCharFormat.setBackground(font.background);
    textCharFormat.setForeground(font.foreground);

    /* Here, alignment and indentation are set. */
    ui->textEdit->document()->blockSignals(true);
    if (alignment != QString::number(firstList.at(1).toInt()) ||
        indentation != QString::number(firstList.at(2).toInt())) {
        QTextBlockFormat textBlockFormat = cursor.block().blockFormat();
        switch (firstList.at(1).toInt()) {
            case 1:
                textBlockFormat.setAlignment(Qt::AlignLeft);
                this->on_actionLeft_alignment_triggered();
                break;
            case 2:
                textBlockFormat.setAlignment(Qt::AlignRight);
                this->on_actionRight_alignment_triggered();
                break;
            case 8:
                textBlockFormat.setAlignment(Qt::AlignJustify);
                this->on_actionJustify_triggered();
                break;
            case 132:
                textBlockFormat.setAlignment(Qt::AlignHCenter);
                this->on_actionCenter_alignment_triggered();
                break;
        }
        textBlockFormat.setIndent(firstList.at(2).toInt());
        cursor.setBlockFormat(textBlockFormat);

        alignment = QString::number(textBlockFormat.alignment());
        indentation = QString::number(textBlockFormat.indent());
    }

    /* Here, there is the actual change of the char. */
    cursor.insertText(character, textCharFormat);
    cursor.setPosition(originalPosition);

    ui->textEdit->document()->blockSignals(false);
}

/* This slot is called when remove_from_window signal is emitted, it has to delete the char
 * from the editor of the other clients. In fact, it accepts the position of the char that
 * must be deleted. */
void TextEditor::delete_text(_int pos) {
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
    /* If the file is opened for the first time nothing should happen. */
    if (openFile) {
        return;
    }
    if (pos == 0) {
        QTextCharFormat textCharFormat = ui->textEdit->currentCharFormat();
        textCharFormat.setBackground(Qt::white);
        ui->textEdit->setCurrentCharFormat(textCharFormat);
    }

    /* Here, the format of the char is taken. */
    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.setPosition(cursor.selectionEnd(), QTextCursor::MoveAnchor);
    QTextCharFormat textCharFormat = cursor.charFormat();
    QTextBlockFormat textBlockFormat = cursor.block().blockFormat();

    QString result = textCharFormat.font().toString() + "/"
                     + QString::number(textBlockFormat.alignment()) + "/"
                     + QString::number(textBlockFormat.indent());

    Symbol::CharFormat charData;
    //charData.background = textCharFormat.background().color();
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
        if (add == del) {
            /* The for loop is used to check if some chars are changed or if the slot is called only because
             * the format of one or more chars is changed. */
            ui->textEdit->undo();
            QString removed = ui->textEdit->toPlainText().mid(pos, add);
            ui->textEdit->redo();
            QString added = ui->textEdit->toPlainText().mid(pos, add);
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
                    multipleErase(pos, del);
                    multipleInsert(pos, added);
                } else {
                    alignment = QString::number(textBlockFormat.alignment());
                    indentation = QString::number(textBlockFormat.indent());

                    qDebug() << alignment << "\n";

                    /* Prova pezza */
                    this->client->localInsert(pos, "X", charData);
                    this->client->localErase(pos);
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
        QString added = ui->textEdit->toPlainText().mid(pos, add);
        if (del == maxPos + 1) {
            for (int i = pos + del - 2; i >= pos; i--) {
                this->client->localErase(i);
            }
            multipleInsert(pos, added);
            return;
        }
        multipleErase(pos, del);
        multipleInsert(pos, added);
    }

}

/* This function is invoked when it is necessary to add more than one char. */
void TextEditor::multipleInsert(int pos, const QString& added) {
    for (QString c : added) {
        QTextCursor cursor(ui->textEdit->textCursor());
        cursor.setPosition(pos+1, QTextCursor::MoveAnchor);
        QTextCharFormat textCharFormat = cursor.charFormat();
        QTextBlockFormat textBlockFormat = cursor.block().blockFormat();

        QString result = textCharFormat.font().toString() + "/"
                         + QString::number(textBlockFormat.alignment()) + "/"
                         + QString::number(textBlockFormat.indent());

        Symbol::CharFormat charData;
        //charData.background = textCharFormat.background().color();
        charData.foreground = textCharFormat.foreground().color();
        charData.font = result;
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





/* This function is used just to draw the button of lists, and within it the corresponding slot is called */
/*void MainWindow::drawListButton() {

    *//* A menù is created and a layout is setted on it *//*
    QMenu *menu = new QMenu(this);
    QGridLayout *gl = new QGridLayout();
    menu->setLayout(gl);

    *//* This is the standard size of all buttons *//*
    const QSize btnSize = QSize(70, 70);

    *//* 6 buttons are created *//*
    QPushButton *b1 = new QPushButton();
    b1->setFixedSize(btnSize);
    QPixmap pixmap1(":/Icons/Disc2.png");
    QIcon ButtonIcon1(pixmap1);
    b1->setIcon(ButtonIcon1);
    b1->setIconSize(btnSize);
    QString buttonStyle = "QPushButton{border:none;background-color:rgba(240, 240, 240,100);}";
    b1->setStyleSheet(buttonStyle);

    QPushButton *b2 = new QPushButton();
    b2->setFixedSize(btnSize);
    QPixmap pixmap2(":/Icons/Circle.png");
    QIcon ButtonIcon2(pixmap2);
    b2->setIcon(ButtonIcon2);
    b2->setIconSize(btnSize);
    b2->setStyleSheet(buttonStyle);

    QPushButton *b3 = new QPushButton();
    b3->setFixedSize(btnSize);
    QPixmap pixmap3(":/Icons/Square.png");
    QIcon ButtonIcon3(pixmap3);
    b3->setIcon(ButtonIcon3);
    b3->setIconSize(btnSize);
    b3->setStyleSheet(buttonStyle);

    QPushButton *b4 = new QPushButton();
    b4->setFixedSize(btnSize);
    QPixmap pixmap4(":/Icons/Decimal.png");
    QIcon ButtonIcon4(pixmap4);
    b4->setIcon(ButtonIcon4);
    b4->setIconSize(btnSize);
    b4->setStyleSheet(buttonStyle);

    QPushButton *b5 = new QPushButton();
    b5->setFixedSize(btnSize);
    QPixmap pixmap5(":/Icons/UpperAlpha.png");
    QIcon ButtonIcon5(pixmap5);
    b5->setIcon(ButtonIcon5);
    b5->setIconSize(btnSize);
    b5->setStyleSheet(buttonStyle);

    QPushButton *b6 = new QPushButton();
    b6->setFixedSize(btnSize);
    QPixmap pixmap6(":/Icons/UpperRoman.png");
    QIcon ButtonIcon6(pixmap6);
    b6->setIcon(ButtonIcon6);
    b6->setIconSize(btnSize);
    b6->setStyleSheet(buttonStyle);

    *//* Buttons are added to menù layout *//*
    gl->addWidget(b1, 0, 0, 1, 1);
    gl->addWidget(b2, 0, 1, 1, 1);
    gl->addWidget(b3, 0, 2, 1, 1);
    gl->addWidget(b4, 1, 0, 1, 1);
    gl->addWidget(b5, 1, 1, 1, 1);
    gl->addWidget(b6, 1, 2, 1, 1);

    *//* Buttons are connected with the slot by passing an Int that indicates the kind of list *//*
    QObject::connect(b1, &QPushButton::clicked, [this]() { MainWindow::listSlot(1); });
    QObject::connect(b2, &QPushButton::clicked, [this]() { MainWindow::listSlot(2); });
    QObject::connect(b3, &QPushButton::clicked, [this]() { MainWindow::listSlot(3); });
    QObject::connect(b4, &QPushButton::clicked, [this]() { MainWindow::listSlot(4); });
    QObject::connect(b5, &QPushButton::clicked, [this]() { MainWindow::listSlot(5); });
    QObject::connect(b6, &QPushButton::clicked, [this]() { MainWindow::listSlot(6); });

    *//* An action is created for the toolButton in order to define a default list *//*
    QAction *act0 = new QAction(this);
    QIcon icon0;
    icon0.addFile(QString::fromUtf8(":/Icons/list.png"), QSize(), QIcon::Normal, QIcon::On);
    act0->setIcon(icon0);

    *//* toolButton is created and added to the toolbar *//*
    QToolButton *toolButton = new QToolButton(this);
    toolButton->setMenu(menu);
    toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    toolButton->setDefaultAction(act0);
    ui->toolBar->addWidget(toolButton);

    *//* The toolButton is connected to the slot *//*
    QObject::connect(toolButton, &QToolButton::clicked, [this]() { MainWindow::listSlot(1); });
};*/

/*void MainWindow::listSlot(int value) {
    *//* Depending on the value passed from drawListButton function, the ListFormat is setted *//*
    QTextListFormat::Style style = QTextListFormat::ListDisc;
    switch (value) {
        default:
            ui->textEdit->setText("ciao");
            break;
        case 1:
            style = QTextListFormat::ListDisc;
            break;
        case 2:
            style = QTextListFormat::ListCircle;
            break;
        case 3:
            style = QTextListFormat::ListSquare;
            break;
        case 4:
            style = QTextListFormat::ListDecimal;
            break;
        case 5:
            style = QTextListFormat::ListUpperAlpha;
            break;
        case 6:
            style = QTextListFormat::ListUpperRoman;
            break;
    }

    *//* The list is created with the format selected *//*
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.beginEditBlock();
    QTextBlockFormat blockFmt = cursor.blockFormat();

    QTextListFormat listFmt;

    if (cursor.currentList()) {
        listFmt = cursor.currentList()->format();
    } else {
        listFmt.setIndent(blockFmt.indent() + 1);
        blockFmt.setIndent(0);
        cursor.setBlockFormat(blockFmt);
    }

    listFmt.setStyle(style);
    cursor.createList(listFmt);
    cursor.endEditBlock();
}*/


/* These are the funcionality implemented in the first toolBar */
/*void MainWindow::on_actionImmagine_triggered() {
    *//* DA SISTEMARE O ELIMINARE *//*
    QString file = QFileDialog::getOpenFileName(this, tr("Select an image"),
                                                ".", tr("Bitmap Files (*.bmp)\n"
                                                        "JPEG (*.jpg *jpeg)\n"
                                                        "GIF (*.gif)\n"
                                                        "PNG (*.png)\n"));

    QUrl Uri(QString("file://%1").arg(file));
    QImage image = QImageReader(file).read();

    QTextDocument *textDocument = ui->textEdit->document();
    textDocument->addResource(QTextDocument::ImageResource, Uri, QVariant(image));
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth(200);
    imageFormat.setHeight(200);
    imageFormat.setName(Uri.toString());
    cursor.insertImage(imageFormat);
}*/


/*void MainWindow::on_actionTabella_triggered() {
    *//* DA SISTEMARE O ELIMINARE *//*
    this->qd->setMinimumSize(200, 100);
    QGridLayout *gl = new QGridLayout;
    this->qd->setLayout(gl);
    QLabel *labelR = new QLabel("Righe");
    QLabel *labelC = new QLabel("Colonne");
    QLabel *labelCWidth = new QLabel("Larghezza colonna");
    QLabel *labelAll = new QLabel("Allineamento");
    QPushButton *ok = new QPushButton("OK");
    this->cWidth->setMinimum(10);
    this->cWidth->setMaximum(1000);
    this->comboBox->addItem("Sinista", QVariant(1));
    this->comboBox->addItem("Centro", QVariant(2));
    this->comboBox->addItem("Destra", QVariant(3));
    this->comboBox->addItem("Giustificato", QVariant(4));
    gl->addWidget(labelR, 0, 0, 1, 1);
    gl->addWidget(this->lineR, 0, 1, 1, 1);
    gl->addWidget(labelC, 1, 0, 1, 1);
    gl->addWidget(this->lineC, 1, 1, 1, 1);
    gl->addWidget(labelCWidth, 2, 0, 1, 1);
    gl->addWidget(this->cWidth, 2, 1, 1, 1);
    gl->addWidget(labelAll, 3, 0, 1, 1);
    gl->addWidget(this->comboBox, 3, 1, 1, 1);
    gl->addWidget(ok, 4, 0, 1, 2);

    QObject::connect(ok, &QPushButton::clicked,
                     [this]() {
                         this->row = this->lineR->value();
                         this->column = this->lineC->value();
                         int width = this->cWidth->value();
                         int all = this->comboBox->itemData(comboBox->currentIndex()).toInt();
                         Qt::Alignment allValue;

                         switch (all) {
                             case 1:
                                 allValue = Qt::AlignLeft;
                                 break;
                             case 2:
                                 allValue = Qt::AlignHCenter;
                                 break;
                             case 3:
                                 allValue = Qt::AlignRight;
                                 break;
                             case 4:
                                 allValue = Qt::AlignJustify;
                                 break;
                         }


                         QTextCursor cursor(ui->textEdit->textCursor());
                         QTextTable *table = cursor.insertTable(this->row, this->column);

                         QTextTableFormat format = table->format();
                         format.setCellPadding(0);
                         format.setCellSpacing(0);
                         format.setWidth(this->column * width);
                         format.setAlignment(allValue);

                         table->setFormat(format);

                         this->qd->close();
                     });

    this->qd->show();
}*/


/*
void MainWindow::on_actionSave_as_triggered() {
    */
/* QUESTO ANDREBBE ELIMINATO PERCHE' IL FILE SI DEVE SALVARE IN AUTOMATICO OGNI TOT SECONDI *//*

    QString filename = QFileDialog::getSaveFileName(this, "Save as..");
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
        return;
    }
    currentFile = filename;
    setWindowTitle(filename);
    QTextStream out(&file);
    QString text = ui->textEdit->document()->toHtml();
    //QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}*/


/*
void MainWindow::on_actionNew_triggered() {
    *//* SI DOVREBBE ELIMINARE *//*
    currentFile.clear();
    ui->textEdit->setText(QString());
}


void MainWindow::on_actionOpen_triggered() {
    *//* QUESTO ANDREBBE ELIMINATO PERCHE' SE VOGLIAMO EDITARE UN NUOVO FILE DOBBIAMO TORNARE NEL PROFILO DELL'UTENTE *//*

    openFile = true;
    QFileDialog fileDialog(this, tr("Open File..."));
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setMimeTypeFilters(QStringList() << "text/html");
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    const QString fn = fileDialog.selectedFiles().first();
    if (load(fn))
        statusBar()->showMessage(tr("Opened \"%1\"").arg(QDir::toNativeSeparators(fn)));
    else
        statusBar()->showMessage(tr("Could not open \"%1\"").arg(QDir::toNativeSeparators(fn)));
}*/

/*bool MainWindow::load(const QString &f) {
    if (!QFile::exists(f))
        return false;
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    //if (Qt::mightBeRichText(str)) {
    QUrl baseUrl = (f.front() == QLatin1Char(':') ? QUrl(f) : QUrl::fromLocalFile(f)).adjusted(QUrl::RemoveFilename);
    ui->textEdit->document()->setBaseUrl(baseUrl);
    ui->textEdit->setHtml(str);
    openFile = false;
    //}

    //setCurrentFileName(f);
    return true;
}*/
