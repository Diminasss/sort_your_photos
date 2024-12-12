#include "IsImageFile.h"

bool isImageFile(const fs::path& filePath) {
    // Для упрощения проверки считаем, что изображения имеют расширения .jpg, .jpeg, .png, .bmp и т. д.
    std::vector<std::string> imageExtensions = { ".jpg", ".jpeg", ".png", ".bmp", ".tiff", ".gif", ".dng" };

    std::string extension = filePath.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower); // Приводим расширение к нижнему регистру

    return std::find(imageExtensions.begin(), imageExtensions.end(), extension) != imageExtensions.end();
}