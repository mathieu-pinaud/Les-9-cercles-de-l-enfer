#include "window.hpp"

int WindowQT::windows(int argc, char *argv[]) {
    
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("My Window");
    window.resize(800, 600);
    window.show();

    return app.exec();
}