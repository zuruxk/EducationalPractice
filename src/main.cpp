#include <QApplication>
#include <iostream>
#include "MainWindow.hpp"

int main(int argc, char *argv[]) {
    try{
        QApplication app(argc, argv);
        MainWindow window;
        window.show();
        return app.exec();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}