#include "writeLog.h"

// Функция для записи лога. Обязательно использовать вместе с #define writeLog(msg) writeLog(msg, __FILE__, __LINE__)
void writeLog(const QString& message, const char* file, int line) {
#ifdef DEBUG
    
    fs::path filePath(file);
    std::string fileName = filePath.filename().string();

    std::ofstream logFile("log.txt", std::ios_base::app);
    if (logFile.is_open()) {
        logFile << "File: " << fileName << ", Line: " << line << " - " << message.toStdString() << std::endl;
    }
    else {
        qDebug() << "Error opening log file!";
    }
#endif
}