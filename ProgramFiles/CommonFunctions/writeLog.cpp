#include "writeLog.h"
#define DEBUG

Q_LOGGING_CATEGORY(myCategory, "ВЫВОД ПРОГРАММЫ")


// Функция для записи лога. Обязательно использовать вместе с #define writeLog(msg) writeLog(msg, __FILE__, __LINE__)
void writeLog(const QString& message, const char* file, int line) {
#ifdef DEBUG    
    qInfo("");
    qInfo() << "[" << file << ":" << line << "] " << message;
    qInfo("");
#endif
}