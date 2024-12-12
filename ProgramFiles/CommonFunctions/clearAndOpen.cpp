#include "clearAndOpen.h"

void clearLogFileAndOpenNotepad() {
    // Очищаем файл логов при запуске
    std::ofstream logFile("log.txt", std::ios_base::trunc);  // Открываем для перезаписи
    if (logFile.is_open()) {
        logFile << "Application started\n"; // Записываем стартовое сообщение
    }
    else {
        qDebug() << "Error opening log file!";
    }

    // Запускаем Блокнот и открываем файл log.txt
    QProcess::startDetached("notepad.exe", QStringList() << "log.txt");
}