#include <iostream>
#include <QtWidgets/QApplication>
#include <server/model/Model.h>
#include <server/Server.h>

int main(int argc, char** argv) {
    std::cout << "Hello, World!" << std::endl;

    QApplication app(argc, argv);
    Model model;
    Server server(9999,model);

    return app.exec();
}
