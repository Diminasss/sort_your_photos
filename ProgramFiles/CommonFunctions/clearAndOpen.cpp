#include "clearAndOpen.h"

// Функция для очситки лог-файла и автоматического открывания блокнота
void clearLogFileAndOpenNotepad() {
#ifdef DEBUG
    std::ofstream logFile("log.txt", std::ios_base::trunc);  
    if (logFile.is_open()) {
        logFile << "Application started\n"; 
    }
    else {
        qDebug() << "Error opening log file!";
    }
    QProcess::startDetached("notepad.exe", QStringList() << "log.txt");
#endif
}