#include <iostream>
#include <QCoreApplication>
#include "virgilium_client.h"
#include <chrono>
#include <thread>
int main(int argc, char** argv) {
    std::cout << "Ciao mamma!" << std::endl;
    std::cout<<"ciaoooo\n";
    std::cout<<"ciao bello\n";
    std::cout<<"ciao 18 marzo 2020\n";
    QApplication miaApp(argc,argv);
    QApplication::setApplicationDisplayName(virgilium_client::tr("client di prova"));

    virgilium_client client(nullptr,"127.0.0.1",9999);
    client.show();
    return miaApp.exec();

}
