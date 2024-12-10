#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Создаём главное окно
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
