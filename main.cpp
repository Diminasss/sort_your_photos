#include "frontend_sort_photo/mainwindow.h"
#include "ProgramFiles/CommonFunctions/writeLog.h"

#include <QApplication>

#define writeLog(msg) writeLog(msg, __FILE__, __LINE__)

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    writeLog("Приложение запущено. Application launched");
    
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}