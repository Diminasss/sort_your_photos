#include "SanitizeFolderName.h"

// Функция для удаления недопустимых символов из имени папки
std::string sanitizeFolderName(const std::string& name) {
    std::string sanitized = name;
    std::string invalidChars = "\\/:*?\"<>|";

    for (char& c : sanitized) {
        if (invalidChars.find(c) != std::string::npos) {
            c = '_';
        }
    }

    return sanitized;
}