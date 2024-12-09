#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Создаём главное окно
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}


// #include "mainwindow.h"
// #include <QApplication>

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);

//     MainWindow w;
//     w.show();

//     return a.exec();
// }
