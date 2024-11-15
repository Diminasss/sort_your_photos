#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // QApplication a(argc, argv);
    // Sort_front w;
    // w.show();
    // return a.exec();

    QApplication app(argc, argv);

    // Create and show the custom FileSend widget
    FileSendWidget fileSendWidget;
    fileSendWidget.show();

    return app.exec();
}
