#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QComboBox>
#include <QFontComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <iostream>
#include <QTextList>
#include <QToolButton>
#include <QWidgetAction>
#include <QTextTable>
#include <QImageReader>
#include <QColorDialog>
#include <QChar>

/* This is the implementation of the constructor */
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);

    this->setMinimumSize(600, 600);     // Window minimum size
    ui->textEdit->setStyleSheet("QTextEdit { padding-left:150; padding-right:150;}");    // left and right padding

    /* wrapper has an horizontal layout, in which the first item is the label with the image, and the second
    item is the wrapper2, that contains file and user names, menubar and toolbar */
    QWidget* wrapper = new QWidget();
    QLayout* hBox = new QHBoxLayout();
    wrapper->setLayout(hBox);
    QWidget* wrapper2 = new QWidget();
    wrapper2->setLayout(new QVBoxLayout());


    QLabel* label = new QLabel();  // label for file image
    label->setPixmap(QPixmap(":/Icons/file.png"));
    label->setFixedWidth(50);
    label->setFixedHeight(50);
    label->setScaledContents(true);

    /* data are insert in the wrapper */
    wrapper->layout()->addWidget(label);
    wrapper->layout()->addWidget(wrapper2);

    /* Here, wrapper2 is populated */
    wrapper2->layout()->setContentsMargins(0,0,0,0);
    QString name = "Nome utente";
    QString filename = "Nome del file";
    QLabel* namel = new QLabel("   " + name + " - " + filename);
    wrapper2->layout()->addWidget(namel);
    wrapper2->layout()->addWidget(ui->menubar);
    wrapper2->layout()->addWidget(ui->toolBar);
    ui->menubar->setStyleSheet("background: #F0F0F0;");

    /* A second toolbar is insert above the first one and again in wrapper2 */
    ui->toolBar = addToolBar(tr("Format Actions"));
    ui->toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(ui->toolBar);
    wrapper2->layout()->addWidget(ui->toolBar);
    this->layout()->setMenuBar(wrapper);

    /* From this point, all items on the second toolbar are inserted */
    /* Font size comboBox */
    drawFontSizeComboBox();
    ui->toolBar->addSeparator();

    /* Font comboBox, color menù and color highligth menù */
    drawFontComboBox();
    drawColorButton();
    drawHighlighterButton();
    ui->toolBar->addSeparator();

    /* List menù */
    drawListButton();


    this->client= new virgilium_client(nullptr,"127.0.0.1",9999);
    //connect (ilClientDiAle, ilSegnale, questaFinestra, loSlot)
    QObject::connect(this->client,&virgilium_client::insert_into_window,this,&MainWindow::change_text);

    /* La prima volta che accedo al client arrivo in questo punto e mi prendo il testo di partenza */
    textValue = ui->textEdit->document()->toPlainText();
}


/* These are the funcionality implemented in the second toolBar */
/* This function is used just to draw the font size comboBox, and within it the corresponding slot is called */
void MainWindow::drawFontSizeComboBox(){

    /* The comboBox is created */
    QComboBox *combosize = new QComboBox(ui->toolBar);
    combosize->setObjectName("comboSize");
    combosize->setEditable(true);

    /* The item are added from a standard list of different size */
    const QList<int> standardSizes = QFontDatabase::standardSizes();
    for (int size : standardSizes) {
        combosize->addItem(QString::number(size));
    }

    /* The default value is setted and the comboBox is connected to the corresponding slot */
    /* SISTEMARE PER INDICARE LA REALE DIMENSIONE DEL FONT */
    combosize->setCurrentIndex(6);
    ui->toolBar->addWidget(combosize);
    connect(combosize, SIGNAL(currentIndexChanged(QString)), SLOT(changeFontSize(QString)));

}

void MainWindow::changeFontSize(const QString& selected)
{
    ui->textEdit->setFontPointSize(selected.toInt());
}


/* This function is used just to draw the font comboBox */
void MainWindow::drawFontComboBox(){

    /* The fontComboBox is created */
    QFontComboBox* myFont = new QFontComboBox;
    ui->toolBar->addWidget(myFont);

    /* The comboBox is connected to a lambda functionthat change the font of the text */
    QObject::connect(myFont, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
                             [=](const QString &text){
        QTextCursor cursor(ui->textEdit->textCursor());
        if(!cursor.hasSelection()){
            ui->textEdit->setFont(text);
        } else {
            ui->textEdit->setCurrentFont(text);
        }
    });

}


/* This function is used just to draw the button of color text, and within it the corresponding slot is called */
void MainWindow::drawColorButton(){

    /* General widget is created */
    QWidget * qw = new QWidget();
    QVBoxLayout *vl = new QVBoxLayout();
    vl->setSpacing(2);
    qw->setLayout(vl);

    /* Here, the button is designed */
    const QSize btnSize = QSize(15, 15);
    QPushButton *b = new QPushButton();
    b->setFixedSize(btnSize);
    QPixmap pixmap1(":/Icons/color.png");
    QIcon ButtonIcon1(pixmap1);
    b->setIcon(ButtonIcon1);
    b->setIconSize(btnSize);
    QString buttonStyle = "QPushButton{border:none;}";
    b->setStyleSheet(buttonStyle);

    /* A label is inserted above the button in order to show the selected color */
    textColorLabel = new QLabel();
    QColor color = ui->textEdit->textColor();
    QString colorS = color.name();
    QString labelStyle = "QLabel{background-color: "+ colorS +"}";
    textColorLabel->setStyleSheet(labelStyle);
    textColorLabel->setFixedWidth(15);
    textColorLabel->setFixedHeight(4);

    /* Button and Label are inserted in the widget layout */
    vl->addWidget(b);
    vl->addWidget(textColorLabel);

    /* widget is inserted on the toolbar */
    ui->toolBar->addWidget(qw);

    /* The button is connected to the slot that open the QColorDialog */
    QObject::connect(b, &QToolButton::clicked, [this](){ MainWindow::changeColorSlot(); });
}

void MainWindow::changeColorSlot(){

    /* The selected color is insert in color variable */
    QColor color = QColorDialog::getColor(Qt::yellow, this );
    /* If color is valid text and label are changed */
    if( color.isValid() ){
          ui->textEdit->setTextColor(color.name());
          QString labelStyle = "QLabel{background-color: "+ color.name() +"}";
          textColorLabel->setStyleSheet(labelStyle);
    }

}


/* This function is used just to draw the button of highlighter, and within it the corresponding slot is called */
void MainWindow::drawHighlighterButton(){
    /* A grid layout is setted to the menù */
    QMenu* menu = new QMenu(this);
    QGridLayout* gl = new QGridLayout();
    menu->setLayout(gl);

    const QSize btnSize = QSize(15, 15);        /* size of each button */

    /* In the for I create, insert and connect the 10 buttons */
    QList<QString> colors = {"white", "green", "cyan", "blue", "red", "magenta", "gray", "darkBlue", "yellow", "black"};
    for(int i = 0; i < 10; i++){
        QPushButton *b = new QPushButton();
        b->setFixedSize(btnSize);
        QString buttonStyle = "QPushButton{background-color:"+ colors[i] +";}";
        b->setStyleSheet(buttonStyle);
        if(i < 5){
            gl->addWidget(b,0,i,1,1);
        } else {
            gl->addWidget(b,1,i-5,1,1);
        }
        QObject::connect(b, &QPushButton::clicked, [this, i](){ MainWindow::highlightSlot(i+1); });
    }

    /* Button on the toolbar is setted */
    QAction* act0 = new QAction(this);
    QIcon icon0;
    icon0.addFile(QString::fromUtf8(":/Icons/highlighter.png"), QSize(), QIcon::Normal, QIcon::On);
    act0->setIcon(icon0);

    /* Menù is inserted in the button, which is addes to the toolbar */
    QToolButton* toolButton = new QToolButton(this);
    toolButton->setMenu(menu);
    toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    toolButton->setDefaultAction(act0);
    QObject::connect(toolButton, &QToolButton::clicked, [this](){ MainWindow::highlightSlot(1); });
    ui->toolBar->addWidget(toolButton);

}

void MainWindow::highlightSlot(int color){

    QTextCharFormat fmt;
    /* Depending on the value passed from drawHighlighterButton function, fmt is setted */
    switch(color){
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

    /* Now, that the fmt variable is ready, text is changed by using cursor and hasSelection method that returns
    true is some text is selected */
    QTextCursor cursor(ui->textEdit->textCursor());
    if(cursor.hasSelection()){
        cursor.setCharFormat(fmt);
    }

}


/* This function is used just to draw the button of lists, and within it the corresponding slot is called */
void MainWindow::drawListButton(){

    /* A menù is created and a layout is setted on it */
    QMenu* menu = new QMenu(this);
    QGridLayout* gl = new QGridLayout();
    menu->setLayout(gl);

    /* This is the standard size of all buttons */
    const QSize btnSize = QSize(70, 70);

    /* 6 buttons are created */
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

    /* Buttons are added to menù layout */
    gl->addWidget(b1,0,0,1,1);
    gl->addWidget(b2,0,1,1,1);
    gl->addWidget(b3,0,2,1,1);
    gl->addWidget(b4,1,0,1,1);
    gl->addWidget(b5,1,1,1,1);
    gl->addWidget(b6,1,2,1,1);

    /* Buttons are connected with the slot by passing an Int that indicates the kind of list */
    QObject::connect(b1, &QPushButton::clicked, [this](){ MainWindow::listSlot(1); });
    QObject::connect(b2, &QPushButton::clicked, [this](){ MainWindow::listSlot(2); });
    QObject::connect(b3, &QPushButton::clicked, [this](){ MainWindow::listSlot(3); });
    QObject::connect(b4, &QPushButton::clicked, [this](){ MainWindow::listSlot(4); });
    QObject::connect(b5, &QPushButton::clicked, [this](){ MainWindow::listSlot(5); });
    QObject::connect(b6, &QPushButton::clicked, [this](){ MainWindow::listSlot(6); });

    /* An action is created for the toolButton in order to define a default list */
    QAction* act0 = new QAction(this);
    QIcon icon0;
    icon0.addFile(QString::fromUtf8(":/Icons/list.png"), QSize(), QIcon::Normal, QIcon::On);
    act0->setIcon(icon0);

    /* toolButton is created and added to the toolbar */
    QToolButton* toolButton = new QToolButton(this);
    toolButton->setMenu(menu);
    toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    toolButton->setDefaultAction(act0);
    ui->toolBar->addWidget(toolButton);

    /* The toolButton is connected to the slot */
    QObject::connect(toolButton, &QToolButton::clicked, [this](){ MainWindow::listSlot(1); });

};

void MainWindow::listSlot(int value)
{
    /* Depending on the value passed from drawListButton function, the ListFormat is setted */
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

    /* The list is created with the format selected */
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

}




/* These are the funcionality implemented in the first toolBar */
void MainWindow::on_actionImmagine_triggered()
{
    /* DA SISTEMARE */
    QString file = QFileDialog::getOpenFileName(this, tr("Select an image"),
                                      ".", tr("Bitmap Files (*.bmp)\n"
                                        "JPEG (*.jpg *jpeg)\n"
                                        "GIF (*.gif)\n"
                                        "PNG (*.png)\n"));

    QUrl Uri ( QString ( "file://%1" ).arg ( file ) );
    QImage image = QImageReader( file ).read();

    QTextDocument * textDocument = ui->textEdit->document();
    textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth( 200 );
    imageFormat.setHeight( 200 );
    imageFormat.setName( Uri.toString() );
    cursor.insertImage(imageFormat);


}


void MainWindow::on_actionTabella_triggered()
{
    this->qd->setMinimumSize(200,100);
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
    gl->addWidget(labelR,0,0,1,1);
    gl->addWidget(this->lineR,0,1,1,1);
    gl->addWidget(labelC,1,0,1,1);
    gl->addWidget(this->lineC,1,1,1,1);
    gl->addWidget(labelCWidth,2,0,1,1);
    gl->addWidget(this->cWidth,2,1,1,1);
    gl->addWidget(labelAll,3,0,1,1);
    gl->addWidget(this->comboBox,3,1,1,1);
    gl->addWidget(ok,4,0,1,2);

    QObject::connect(ok, &QPushButton::clicked,
                      [this](){
        this->row = this->lineR->value();
        this->column = this->lineC->value();
        int width = this->cWidth->value();
        int all = this->comboBox->itemData(comboBox->currentIndex()).toInt();
        Qt::Alignment allValue;

        switch(all){
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
    } );

    this->qd->show();
}


void MainWindow::on_actionNew_triggered()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
}


void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open the file..");
    QFile file(filename);
    currentFile = filename;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    setWindowTitle(filename);
    QTextStream in(&file);
    QString text = in.readAll();
    ui-> textEdit->setText(text);
    file.close();
}


void MainWindow::on_actionSave_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save as..");
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
        return;
    }
    currentFile = filename;
    setWindowTitle(filename);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionRight_allignment_triggered()
{
    ui->textEdit->setAlignment(Qt::AlignRight);
}


void MainWindow::on_actionLeft_allignment_triggered()
{
    ui->textEdit->setAlignment(Qt::AlignLeft);
}


void MainWindow::on_actionCenter_allignment_triggered()
{
    ui->textEdit->setAlignment(Qt::AlignCenter);
}


void MainWindow::on_actionJustify_triggered()
{
    ui->textEdit->setAlignment(Qt::AlignJustify);
}


void MainWindow::on_actionSelect_all_triggered()
{
    ui->textEdit->selectAll();
}


void MainWindow::on_actionSottolinea_triggered()
{
    if(ui->textEdit->fontUnderline()){
        ui->textEdit->setFontUnderline(false);
    } else {
        ui->textEdit->setFontUnderline(true);
    }
}


void MainWindow::on_actionGrassetto_triggered()
{
    if(ui->textEdit->fontWeight() == 50){
        ui->textEdit->setFontWeight(75);
    } else {
        ui->textEdit->setFontWeight(50);
    }
}


void MainWindow::on_actionCorsivo_triggered()
{
    if(ui->textEdit->fontItalic()){
        ui->textEdit->setFontItalic(false);
    } else {
        ui->textEdit->setFontItalic(true);
    }
}


void MainWindow::on_actionTrova_e_sostituisci_triggered()
{
    QDialog *qd = new QDialog;
    QVBoxLayout *vl = new QVBoxLayout;
    qd->setLayout(vl);
    QLabel *labelT = new QLabel("Trova");
    QLabel *labelS = new QLabel("Sostituisci con");
    QPushButton *ok = new QPushButton("Vai");



    vl->addWidget(labelT);
    vl->addWidget(this->lineT);
    vl->addWidget(labelS);
    vl->addWidget(this->lineS);
    vl->addWidget(ok);


    QObject::connect(ok, &QPushButton::clicked,
                      [this](){
        this->find = this->lineT->text().toUtf8().data();
        this->replace = this->lineS->text().toUtf8().data();

        ui->textEdit->moveCursor(QTextCursor::Start);
        while(ui->textEdit->find(find))
        {
          ui->textEdit->textCursor().insertText(replace);
        }
        this->lineT->clear();
        this->lineS->clear();
    } );

    qd->show();
}


void MainWindow::on_actionAumenta_rientro_triggered()
{
    modificaIndentazione(1);
}


void MainWindow::on_actionDiminuisci_rientro_triggered()
{
    modificaIndentazione(-1);
}


void MainWindow::modificaIndentazione(int num){

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


void MainWindow::on_actionEsporta_PDF_triggered()
{
    /*
#ifndef QT_NO_PRINTER
//! [0]
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
//! [0]
#endif*/
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::change_text(_int pos, QString character) {
    /* Potrebbe essere una buona idea inserire anche un codice che ci dice che operazione è stata fatta
     * in modo tale da gestire l'aggiornamento dell'editor che riceve il cambiamento, con uno switch case simile a
     * quello di quando si fa un cambiamento */

    QString values = ui->textEdit->document()->toPlainText();
    values.insert(pos, character);

    ui->textEdit->blockSignals(true);
    ui->textEdit->setText(values);
    ui->textEdit->blockSignals(false);
}


void MainWindow::on_textEdit_textChanged()
{
    /* Mi prendo il testo aggiornato dopo il signla textChanged() */
    newTextValue = ui->textEdit->document()->toPlainText();

    /* Mi prendo la posizione aggiornata del cursore dopo il signal textChanged() */
    QTextCursor cursor = ui->textEdit->textCursor();
    int point = cursor.position();
    _int p = point;

    /* Faccio uno switch case per capire in che situazione mi trovo:
     * 0: Cambiamento del formato del testo o sostituzione di un carattere con un altro (esempio taglia/incolla)
     * 1: Inserisce un singolo carattere
     * -1: Rimuove un singolo carattere
     * >1: Inserisce più di un carattere contemporaneamente (esempio copia/incolla)
     * <1: Rimuove più di un carattere contemporaneamente (esempio selezione->canc)
     * */
    int diff = newTextValue.length() - textValue.length();
    switch(diff){
        case 0: {
            /* In questo caso la dimensione non è cambiata */
            break;
        }
        case 1: {
            /* newTextValue ha un carattere in più di textValue, quindi è stato inserito un carattere */
            const QString c = newTextValue.at(point - 1);
            this->client->localInsert(p - 1, c);
            qDebug()<< "Inserito il carattere " << c << " dalla posizione del vettore " << p-1;
            break;
        }
        case -1: {
            /* newTextValue ha un carattere in meno di textValue, quindi è stato rimosso un carattere */
            const QString c = textValue.at(point); /* Inserito nel caso in cui ale vuole anche il carattere */
            //this->client->localErase(p);
            qDebug()<< "Rimosso il carattere " << c << " dalla posizione del vettore " << p;
            break;
        }
        default: {
            /* Qui si gestisce il caso di diff > 1 e diff < 1 */
            if(diff > 1){
                QString addedValues = "";
                for(int i = point-diff; i < point; i++){
                    addedValues = addedValues + newTextValue.at(i);
                }
                qDebug()<< "Inseriti i caratteri " << addedValues << " dalla posizione del vettore " << point-diff << " alla posizione " << point-1;
            } else {
                QString removedValues = "";
                for(int i = point; i < point - diff; i++){
                    removedValues = removedValues + textValue.at(i);
                }
                qDebug()<< "Rimossi i caratteri " << removedValues << " dalla posizione del vettore " << point << " alla posizione " << point-diff-1;
            }
        }
    }

    /* Mi aggiorno il valore di textValue */
    textValue = newTextValue;






    /*QDialog *qd = new QDialog;
    QVBoxLayout *vl = new QVBoxLayout;
    qd->setLayout(vl);
    QString str = "E' stata inserita la lettera ";
    QString str2 = " alla  posizione ";
    QString final = str + c + str2 + QString::number(point);
    QLabel *labelT = new QLabel(final);
    vl->addWidget(labelT);

    qd->show();*/
}
