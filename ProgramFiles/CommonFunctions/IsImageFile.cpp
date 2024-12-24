#include "IsImageFile.h"

// Функция, которая проверяет, является ли файл фотографией
bool isImageFile(const fs::path& filePath) {
    std::vector<std::string> imageExtensions = { ".jpg", ".jpeg", ".png", ".bmp", ".tiff", ".gif", ".dng" };

    std::string extension = filePath.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower); // Приводим расширение к нижнему регистру

    return std::find(imageExtensions.begin(), imageExtensions.end(), extension) != imageExtensions.end();
}