#include "ProgramFiles/CommonFunctions/clearAndOpen.h"
#include "frontend_sort_photo/mainwindow.h"

#include <QApplication>

#define writeLog(msg) writeLog(msg, __FILE__, __LINE__)


namespace fs = std::filesystem;


int main(int argc, char* argv[]) {
    clearLogFileAndOpenNotepad();

    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}