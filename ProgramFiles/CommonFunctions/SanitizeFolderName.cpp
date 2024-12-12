#include "SanitizeFolderName.h"

// Функция для удаления недопустимых символов из имени папки
std::string sanitizeFolderName(const std::string& name) {
    std::string sanitized = name;
    // Список символов, которые нельзя использовать в именах файлов/папок в Windows
    std::string invalidChars = "\\/:*?\"<>|";

    // Заменяем все недопустимые символы на подчеркивания
    for (char& c : sanitized) {
        if (invalidChars.find(c) != std::string::npos) {
            c = '_';  // Заменяем недопустимый символ на _
        }
    }

    return sanitized;
}