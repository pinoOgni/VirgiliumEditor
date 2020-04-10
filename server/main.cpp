#include <iostream>
#include <QCoreApplication>
#include "virgilium_server.h"

int main(int argc, char** argv) {
    std::cout<<"Ciao mamma!\n";
    QCoreApplication miaApp(argc,argv);
    virgilium_server server(nullptr);

    
    return miaApp.exec();
}
