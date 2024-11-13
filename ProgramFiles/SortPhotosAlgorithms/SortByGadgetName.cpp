#include "SortByGadgetName.h"

#include "../CommonFunctions/IsImageFile.h"
#include "../CommonFunctions/SanitizeFolderName.h"
#include "../GetPhotosParameters/GetCameraName.h"

#include <iostream>


void SortPhotosByGadgetName(const fs::path& directory, const fs::path& targetDirectory) {
    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_directory()) {
                SortPhotosByGadgetName(entry.path(), targetDirectory);  // Рекурсивный вызов для подкаталогов
            }
            else if (entry.is_regular_file()) {
                std::string filePath = entry.path().string();

                // Проверяем, является ли файл изображением
                if (isImageFile(entry.path())) {
                    // Получаем имя камеры для фотографии
                    std::string cameraName = GetCameraName(filePath);

                    if (cameraName != "NoExifData" && cameraName != "Error") {
                        // Санитизируем имя камеры, чтобы сделать его допустимым для имени папки
                        std::string sanitizedCameraName = sanitizeFolderName(cameraName);

                        fs::path newDir = targetDirectory / sanitizedCameraName;  // Папка с именем камеры
                        fs::create_directories(newDir);  // Создаем папку, если её нет

                        // Формируем новый путь для перемещения
                        fs::path newPath = newDir / entry.path().filename();

                        // Перемещаем файл
                        fs::rename(entry.path(), newPath);
                        std::cout << "Перемещено: " << filePath << " -> " << newPath << std::endl;
                    }
                    else {
                        // Логируем ошибку, если EXIF-данные отсутствуют
                        std::cerr << "Ошибка обработки изображения (No EXIF data): " << filePath << std::endl;

                        // Перемещаем в папку NoExifData
                        fs::path noExifDataDir = targetDirectory / "NoExifData";
                        fs::create_directories(noExifDataDir);
                        fs::path newPath = noExifDataDir / entry.path().filename();
                        fs::rename(entry.path(), newPath);
                    }
                }
                else {
                    // Логируем ошибку, если файл не является изображением
                    std::cerr << "Не является изображением: " << filePath << std::endl;

                    // Перемещаем в папку NotPhotos
                    fs::path notPhotosDir = targetDirectory / "NotPhotos";
                    fs::create_directories(notPhotosDir);
                    fs::path newPath = notPhotosDir / entry.path().filename();
                    fs::rename(entry.path(), newPath);
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
}