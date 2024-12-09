#include "SortByGadgetName.h"

#include "../CommonFunctions/IsImageFile.h"
#include "../CommonFunctions/SanitizeFolderName.h"
#include "../GetPhotosParameters/GetCameraName.h"

#include <iostream>

#include <filesystem>
#include <thread>
#include <vector>
#include <mutex>

namespace fs = std::filesystem;

// Мьютекс для синхронизации вывода (если нужно логировать работу потоков)
std::mutex coutMutex;

// Функция для обработки части файлов
void processFiles(const std::vector<fs::path>& files, const fs::path& targetDirectory) {
    for (const auto& filePath : files) {
        try {
            // Проверяем, является ли файл изображением
            if (isImageFile(filePath)) {
                // Получаем имя камеры для фотографии
                std::string cameraName = GetCameraName(filePath.string());

                if (cameraName != "NoExifData" && cameraName != "Error") {
                    std::string sanitizedCameraName = sanitizeFolderName(cameraName);

                    fs::path newDir = targetDirectory / sanitizedCameraName;  // Папка с именем камеры
                    fs::create_directories(newDir);  // Создаем папку, если её нет

                    fs::path newPath = newDir / filePath.filename();
                    fs::rename(filePath, newPath);

                    std::lock_guard<std::mutex> lock(coutMutex);
                    // std::cout << "Перемещено: " << filePath << " -> " << newPath << std::endl;
                }
                else {
                    fs::path noExifDataDir = targetDirectory / "NoExifData";
                    fs::create_directories(noExifDataDir);
                    fs::path newPath = noExifDataDir / filePath.filename();
                    fs::rename(filePath, newPath);
                }
            }
            else {
                fs::path notPhotosDir = targetDirectory / "NotPhotos";
                fs::create_directories(notPhotosDir);
                fs::path newPath = notPhotosDir / filePath.filename();
                fs::rename(filePath, newPath);
            }
        }
        catch (const std::exception& e) {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cerr << "Ошибка: " << e.what() << " для файла " << filePath << std::endl;
        }
    }
}

void SortPhotosByGadgetNameParallel(const fs::path& directory, const fs::path& targetDirectory) {
    std::vector<fs::path> allFiles;

    // Сканируем все файлы в директории
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            allFiles.push_back(entry.path());
        }
    }

    // Разбиваем файлы на 4 части
    size_t totalFiles = allFiles.size();
    size_t chunkSize = (totalFiles + 3) / 4;  // Округляем в большую сторону
    std::vector<std::thread> threads;

    for (size_t i = 0; i < 4; ++i) {
        size_t startIdx = i * chunkSize;
        size_t endIdx = std::min(startIdx + chunkSize, totalFiles);

        if (startIdx < totalFiles) {
            std::vector<fs::path> chunk(allFiles.begin() + startIdx, allFiles.begin() + endIdx);
            threads.emplace_back(processFiles, chunk, targetDirectory);
        }
    }

    // Ожидаем завершения всех потоков
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
















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
