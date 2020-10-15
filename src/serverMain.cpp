#include <iostream>
#include <QtWidgets/QApplication>
#include <server/model/Model.h>
#include <server/Server.h>
#include <spdlog/spdlog.h>
#include <common/constants.h>

int main(int argc, char** argv) {

    if (argc < 1) {
        spdlog::info("Usage: {} [-d/--debug] [-t/--test]", argv[0]);
        exit(-1);
    }

    //level of spdlog
    if (argc == 3 && (std::strncmp(argv[1], "-d", 2) == 0 || std::strncmp(argv[1], "--debug", 7) == 0)) {
        spdlog::set_level(spdlog::level::debug);
    } else {
        spdlog::set_level(spdlog::level::critical);
    }
    //testDB
    //if (argc == 3 && (std::strncmp(argv[2], "-t", 2) == 0 || std::strncmp(argv[2], "--test", 6) == 0))


    QApplication app(argc, argv);
    Model model;
    Server server(9999,model);

    return app.exec();
}
