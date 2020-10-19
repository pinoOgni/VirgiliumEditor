#include <QtWidgets/QApplication>
#include <server/model/Model.h>
#include <server/Server.h>

int main(int argc, char **argv) {
    /*if (argc < 1) {
        spdlog::info("Usage: {} [-d/--debug]", argv[0]);
        exit(-1);
    }

    //level of spdlog
    if (argc == 2 && (std::strncmp(argv[1], "-d", 2) == 0 || std::strncmp(argv[1], "--debug", 7) == 0)) {
        spdlog::set_level(spdlog::level::debug);
    }*/

    QApplication app(argc, argv);
    Model model;
    Server server(9999, model);

    return app.exec();
}
