//
// Created by alex on 07/12/19.
//

#include "virgilium_client.h"
#include "symbol.h"
#include "message.h"
#include <algorithm>
#include <iostream>
#include "exceptions.h"

virgilium_client::virgilium_client(QWidget *parent, QString ServerName, qint64 serverPort):QDialog(parent),
    hostCombo(new QComboBox),portLineEdit(new QLineEdit),getFortuneButton(new QPushButton(tr("local insert"))){

    this->server = new server_virgilium(this, ServerName, serverPort, *this);
    this->_counter=0;

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    auto hostLabel = new QLabel(tr("&Server name:"));
    auto portLabel = new QLabel(tr("S&erver port:"));
    hostLabel->setBuddy(hostCombo);
    hostCombo->setEditable(true);
    getFortuneButton->setDefault(true);
    getFortuneButton->setEnabled(true);
    auto quitButton = new QPushButton(tr("Quit"));
    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(getFortuneButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
    QGridLayout *mainLayout = nullptr;
    if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
        auto outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        auto outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QGridLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    } else {
        mainLayout = new QGridLayout(this);
    }
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostCombo, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(statusLabel, 2, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);

    setWindowTitle(QGuiApplication::applicationDisplayName());
    portLineEdit->setFocus();
    /*
     * In apertura della connessione il server mandera' un messaggio con codice:
     * SITE_ID_ASSIGNMENT che contiene l'id del client, quest'ultimo lo assegnerà
     * a this->_siteId
     * Per ulteriori informazioni ficcare il naso nello slot readyread della classe server_virgilium
     *
     * */

    /*
     * effettuo la bind dei segnali:
     * */

    connect(getFortuneButton, &QAbstractButton::clicked,
            this, &virgilium_client::faiLocalInsert);
    connect(this,&virgilium_client::site_id_assignment,this->server,&server_virgilium::site_id_assignment);
}



virgilium_client::~virgilium_client(){
    delete(this->server);
}


_int virgilium_client::get_id() const {
    return this->_siteId;
}

QString virgilium_client::to_string() {
    QString buffer;
    std::for_each(this->_symbols.begin(),this->_symbols.end(),[&buffer](symbol s)  {
        buffer.push_back(s.getLetter());
    });
    return buffer;
}

void virgilium_client::localErase(_int index) {
    auto s = this->_symbols[index];
    auto it = this->_symbols.begin() + index;
    this->_symbols.erase(it);
    message m(s,this->_siteId,"ERASE");
    this->server->send(m,SYMBOL_INSERT_OR_ERASE);
    return;
}
//TODO
void virgilium_client::process(const message &m) {

    // "ERASE" "INSERT"
    auto s = m.getSymbol();
    _int i,index;
    bool flagUguale=false;
    if(m.getAction()=="INSERT"){
        //std::cout<<"Sto facendo una insert\n";
        auto nuovaPos = m.getSymbol().getPosition();
        for (_int i = 0; i < this->_symbols.size(); i++) {
            if (this->_symbols[i].getPosition() == nuovaPos)
                //throw wrongpositionException();
                throw wrongpositionException();
        }


        for(i=0;i<this->_symbols.size(); i++)
            if( s < this->_symbols[i] ) break; // ho trovato il mio i;
        auto it = this->_symbols.begin() + i ;
        this->_symbols.insert(it,s);
    }
    else{
        this->_symbols.erase(
                std::remove_if(this->_symbols.begin(),
                               this->_symbols.end(),
                               [&s](symbol s1){
                                   return s==s1;
                               }
                )
        );

    }
    return;
}

//gli interi prev e next sono quelli che andranno a contenere quelli che sono gli elementi passati alla funzione
//i due vettori vengono controllati parallelemante
//_max gli si assegna il la dimensione maggiore tra i due vettori
QVector<_int> virgilium_client::getPosition(QVector<_int> prec,QVector<_int> succ) {
    QVector<_int> nuovaPos;
    _int i,prev,next,_max = (prec.size() > succ.size()) ? prec.size() : succ.size() ;
    for(i=0;i<_max;i++){
        //size Ã¨ finito? se Ã¨ finito non ci voglio accedere (potrei avere un errore)
        //se un vettore non Ã¨ ancora arrivato alla fine, mi prendo il suo valore
        //se un vettore Ã¨ finito gli assegno zero
        prev = (i>=prec.size()) ?  0 : prec[i];
        next = (i>=succ.size()) ?  9 : succ[i];

        if(prev==next){
            nuovaPos.push_back(prev);
        }
        else{
            if(next-prev >= 2){
                nuovaPos.push_back(prev+1);
                break;
            }
            else{
                // aggiungiamo il caso in cui prev > next
                if(prev > next){

                    // se prev != 9 allora possiamo incrementarlo
                    if(prev != 9){
                        nuovaPos.push_back(prev + 1);
                        break;
                    } else if(next != 0){ // se prev = 9 allora proviamo a decrementare next
                        nuovaPos.push_back(next - 1);
                        break;
                    }
                }
                nuovaPos.push_back(prev);

            }

        }
    }
    if(i==_max){
        nuovaPos.push_back(1);
    }
    return nuovaPos;
}

void virgilium_client::localInsert(_int index, QString value){
    QVector<_int> prec; //= this->_symbols[index-1];
    QVector<_int> nuovaPos;
    QVector<_int> succ; //auto succ //= this-> _symbols[index];



    if(this->_symbols.size()==0 && index==0){
        //primo elemento inserito
        prec.push_back(0);
        succ.push_back(2);
        // nuovaPos.push_back(1);// lo zero non va mai messo come posizione
    }
    else{
        if(index==0){
            //inserisco in testa
            //std::vector<int> zeroes;
            prec.push_back(0);
            succ=this->_symbols[index].getPosition();
        }
        if( index==this->_symbols.size() ){
            //ultimo a destra
            prec=this->_symbols[index-1].getPosition();
            succ.push_back(prec[0]+2);
            // faccio prec[0]+2 così la mia funzione getPosition dovrebbe ficcare
            // il nuovo symbol tra i due senza creare un vettorone ma creando un
            // vettore di un elemento di valore prec[0]+1 (spero)
        }
        if(  index!=0  && index!=this->_symbols.size()  ){
            //caso medio
            //    std::cout<<"sto inserendo " << value<<std::endl;
            prec=this->_symbols[index-1].getPosition();
            succ=this->_symbols[index].getPosition();
        }


    }


    nuovaPos=this->getPosition(prec,succ);

    symbol nuovo(value,this->_siteId,this->_counter++,nuovaPos);
    auto it = this->_symbols.begin() + index;
    this->_symbols.insert(it,nuovo);
    message m(nuovo,this->_siteId,"INSERT");
    std::cout<<"il mio siteID e':"<<this->_siteId<<"sto mandando il messaggio:\n";
    m.printMessage();
    this->server->send(m,SYMBOL_INSERT_OR_ERASE);
    return;
}


void virgilium_client::set_site_id(qint64 siteId) {
    this->_siteId=siteId;
    qDebug() << "Il mio site ID e' : " << this->_siteId <<"\n";
}


void virgilium_client::mandaqualcosa() {
    this->server->mandaqualcosa();
}

void virgilium_client::faiLocalInsert() {
    this->localInsert(0,"c");

}

