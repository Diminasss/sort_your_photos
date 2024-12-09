#include "SortByGadgetName.h"

#include "../CommonFunctions/IsImageFile.h"
#include "../CommonFunctions/SanitizeFolderName.h"
#include "../GetPhotosParameters/GetCameraName.h"

#include <iostream>
#include <filesystem>
#include <thread>
#include <vector>
#include <mutex>

#include <unordered_map>
#include <sstream>
#include <fstream>
#include <openssl/evp.h>


namespace fs = std::filesystem;

// Мьютекс для синхронизации вывода (если нужно логировать работу потоков)
std::mutex coutMutex;

std::string computeFileHash(const fs::path& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        return "";
    }

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create OpenSSL context.");
    }

    const EVP_MD* md = EVP_sha256();
    if (!md) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize SHA256.");
    }

    if (EVP_DigestInit_ex(ctx, md, nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize Digest.");
    }

    char buffer[8192];
    while (file.read(buffer, sizeof(buffer))) {
        if (EVP_DigestUpdate(ctx, buffer, file.gcount()) != 1) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("Failed to update Digest.");
        }
    }
    if (file.gcount() > 0) {
        if (EVP_DigestUpdate(ctx, buffer, file.gcount()) != 1) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("Failed to update Digest.");
        }
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLength = 0;
    if (EVP_DigestFinal_ex(ctx, hash, &hashLength) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize Digest.");
    }

    EVP_MD_CTX_free(ctx);

    std::stringstream hashStream;
    for (unsigned int i = 0; i < hashLength; i++) {
        hashStream << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return hashStream.str();
}

void processFiles(
    const std::vector<fs::path>& files,
    const fs::path& targetDirectory,
    std::unordered_map<std::string, int>& hashMap
) {
    static std::mutex hashMapMutex; // Мьютекс для защиты hashMap

    for (const auto& filePath : files) {
        try {
            if (isImageFile(filePath)) {
                std::string cameraName = GetCameraName(filePath.string());

                if (cameraName != "NoExifData" && cameraName != "Error") {
                    std::string sanitizedCameraName = sanitizeFolderName(cameraName);

                    fs::path newDir = targetDirectory / sanitizedCameraName;
                    fs::create_directories(newDir);

                    // Считаем хеш файла
                    std::string fileHash = computeFileHash(filePath);

                    {
                        std::lock_guard<std::mutex> lock(hashMapMutex);
                        // Проверяем, есть ли уже такой файл с таким хешом
                        if (hashMap.find(fileHash) != hashMap.end()) {
                            int duplicateCount = hashMap[fileHash]++;
                            fs::path duplicateDir = targetDirectory / "Duplicates";

                            // Создаём папку для дубликатов, если её нет
                            fs::create_directories(duplicateDir);

                            fs::path newDuplicatePath = duplicateDir / (sanitizedCameraName + "_duplicate" +
                                std::to_string(duplicateCount) + filePath.extension().string());
                            fs::rename(filePath, newDuplicatePath);

                            std::lock_guard<std::mutex> coutLock(coutMutex);
                            std::cout << "Дубликат перемещен: " << filePath << " -> " << newDuplicatePath << std::endl << std::endl;
                        }
                        else {
                            // Если файл не дубликат, перемещаем его в папку по имени камеры
                            fs::path newPath = newDir / filePath.filename();
                            fs::rename(filePath, newPath);

                            // Добавляем хеш в карту
                            hashMap[fileHash] = 1;

                            std::lock_guard<std::mutex> coutLock(coutMutex);
                            std::cout << "Перемещено: " << filePath << " -> " << newPath << std::endl << std::endl;
                        }
                    }
                }
                else {
                    fs::path noExifDataDir = targetDirectory / "NoExifData";
                    fs::create_directories(noExifDataDir);
                    fs::path newPath = noExifDataDir / filePath.filename();
                    fs::rename(filePath, newPath);

                    std::lock_guard<std::mutex> coutLock(coutMutex);
                    std::cout << "Файл без EXIF данных перемещен: " << filePath << " -> " << newPath << std::endl << std::endl;
                }
            }
            else {
                fs::path notPhotosDir = targetDirectory / "NotPhotos";
                fs::create_directories(notPhotosDir);
                fs::path newPath = notPhotosDir / filePath.filename();
                fs::rename(filePath, newPath);

                std::lock_guard<std::mutex> coutLock(coutMutex);
                std::cout << "Не изображение перемещено: " << filePath << " -> " << newPath << std::endl << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cerr << "Ошибка: " << e.what() << " для файла " << filePath << std::endl;
        }
    }
}


void SortPhotosByGadgetNameParallel(const fs::path& directory, const fs::path& targetDirectory) {
    std::unordered_map<std::string, int> fileHashes; // Карта для хранения хеш-сумм

    std::vector<fs::path> allFiles;
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            allFiles.push_back(entry.path());
        }
    }

    size_t totalFiles = allFiles.size();
    size_t chunkSize = (totalFiles + 3) / 4; // Разбиваем файлы на 4 части
    std::vector<std::thread> threads;

    for (size_t i = 0; i < 4; ++i) {
        size_t startIdx = i * chunkSize;
        size_t endIdx = std::min(startIdx + chunkSize, totalFiles);

        if (startIdx < totalFiles) {
            // Формируем часть файлов для текущего потока
            std::vector<fs::path> chunk(allFiles.begin() + startIdx, allFiles.begin() + endIdx);

            // Запускаем поток с помощью лямбда-функции
            threads.emplace_back([chunk, &targetDirectory, &fileHashes]() {
                processFiles(chunk, targetDirectory, fileHashes);
                });
        }
    }

    // Ожидаем завершения всех потоков
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}





//// Функция для обработки части файлов рабочая без сравнения хеша
//void processFiles(const std::vector<fs::path>& files, const fs::path& targetDirectory) {
//    for (const auto& filePath : files) {
//        try {
//            // Проверяем, является ли файл изображением
//            if (isImageFile(filePath)) {
//                // Получаем имя камеры для фотографии
//                std::string cameraName = GetCameraName(filePath.string());
//
//                if (cameraName != "NoExifData" && cameraName != "Error") {
//                    std::string sanitizedCameraName = sanitizeFolderName(cameraName);
//
//                    fs::path newDir = targetDirectory / sanitizedCameraName;  // Папка с именем камеры
//                    fs::create_directories(newDir);  // Создаем папку, если её нет
//
//                    fs::path newPath = newDir / filePath.filename();
//                    fs::rename(filePath, newPath);
//
//                    std::lock_guard<std::mutex> lock(coutMutex);
//                    // std::cout << "Перемещено: " << filePath << " -> " << newPath << std::endl;
//                }
//                else {
//                    fs::path noExifDataDir = targetDirectory / "NoExifData";
//                    fs::create_directories(noExifDataDir);
//                    fs::path newPath = noExifDataDir / filePath.filename();
//                    fs::rename(filePath, newPath);
//                }
//            }
//            else {
//                fs::path notPhotosDir = targetDirectory / "NotPhotos";
//                fs::create_directories(notPhotosDir);
//                fs::path newPath = notPhotosDir / filePath.filename();
//                fs::rename(filePath, newPath);
//            }
//        }
//        catch (const std::exception& e) {
//            std::lock_guard<std::mutex> lock(coutMutex);
//            std::cerr << "Ошибка: " << e.what() << " для файла " << filePath << std::endl;
//        }
//    }
//}

//void SortPhotosByGadgetNameParallel(const fs::path& directory, const fs::path& targetDirectory) {
//    std::vector<fs::path> allFiles;
//
//    // Сканируем все файлы в директории
//    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
//        if (entry.is_regular_file()) {
//            allFiles.push_back(entry.path());
//        }
//    }
//
//    // Разбиваем файлы на 4 части
//    size_t totalFiles = allFiles.size();
//    size_t chunkSize = (totalFiles + 3) / 4;  // Округляем в большую сторону
//    std::vector<std::thread> threads;
//
//    for (size_t i = 0; i < 4; ++i) {
//        size_t startIdx = i * chunkSize;
//        size_t endIdx = std::min(startIdx + chunkSize, totalFiles);
//
//        if (startIdx < totalFiles) {
//            std::vector<fs::path> chunk(allFiles.begin() + startIdx, allFiles.begin() + endIdx);
//            threads.emplace_back(processFiles, chunk, targetDirectory);
//        }
//    }
//
//    // Ожидаем завершения всех потоков
//    for (auto& thread : threads) {
//        if (thread.joinable()) {
//            thread.join();
//        }
//    }
//}
















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
