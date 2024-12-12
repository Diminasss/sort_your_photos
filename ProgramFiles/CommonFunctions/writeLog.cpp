#include "writeLog.h"

void writeLog(const QString& message, const char* file, int line) {
    // Извлекаем только имя файла без пути
    fs::path filePath(file);
    std::string fileName = filePath.filename().string();

    // Открываем файл для записи (добавление в конец)
    std::ofstream logFile("log.txt", std::ios_base::app);
    if (logFile.is_open()) {
        // Записываем в лог только имя файла и строку
        logFile << "File: " << fileName << ", Line: " << line << " - " << message.toStdString() << std::endl;
    }
    else {
        qDebug() << "Error opening log file!";
    }
}

//void writeLog(const QString& message, const char* file, int line) {
//    std::ofstream logFile("log.txt", std::ios_base::app); // Открываем файл для записи (добавление в конец)
//    if (logFile.is_open()) {
//        // Записываем в лог сообщение с информацией о файле и строке
//        logFile << "File: " << file << ", Line: " << line << " - " << message.toStdString() << std::endl;
//    }
//    else {
//        qDebug() << "Error opening log file!";
//    }
//}
