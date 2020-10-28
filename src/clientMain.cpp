
#include <iostream>
#include "client/view/WelcomePage.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    /*if (argc < 1) {
        spdlog::info("Usage: {} [-d/--debug]", argv[0]);
        exit(-1);
    }

    //level of spdlog
    if (argc == 2 && (std::strncmp(argv[1], "-d", 2) == 0 || std::strncmp(argv[1], "--debug", 7) == 0)) {
        spdlog::set_level(spdlog::level::debug);
    }*/

    freopen("clientLog.txt", "w", stderr);

    QString address;
    if(argc < 2)
        address = "127.0.0.1";
    else
        address = argv[1];

    WelcomePage w(0,address);
    w.show();
    return a.exec();
}
