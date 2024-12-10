#include "SortByGadgetName.h"

#include "../CommonFunctions/IsImageFile.h"
#include "../CommonFunctions/SanitizeFolderName.h"
#include "../GetPhotosParameters/GetCameraName.h"
#include "../CommonFunctions/computeFileHash.h"
#include "../CommonFunctions/getUniquePath.h"


namespace fs = std::filesystem;

// Мьютекс для синхронизации вывода (если нужно логировать работу потоков)
std::mutex coutMutex;


void processFilesWithDevice(
    const std::vector<fs::path>& files,
    const fs::path& targetDirectory,
    std::unordered_map<std::string, int>& hashMap
) {
    static std::mutex hashMapMutex; // Мьютекс для защиты hashMap

    for (const auto& filePath : files) {
        try {
            // Считаем хеш файла
            std::string fileHash = computeFileHash(filePath);

            if (isImageFile(filePath)) {
                // Является картинкой
                std::string cameraName = GetCameraName(filePath.string());

                if (cameraName != "NoExifData" && cameraName != "Error") {
                    // Имеет Exif данные
                    std::string sanitizedCameraName = sanitizeFolderName(cameraName);

                    fs::path newDir = targetDirectory / sanitizedCameraName;
                    fs::create_directories(newDir);

                    {
                        std::lock_guard<std::mutex> lock(hashMapMutex);
                        if (hashMap.find(fileHash) != hashMap.end()) {
                            // Является дубликатом
                            int duplicateCount = hashMap[fileHash]++;
                            fs::path duplicateDir = newDir / "Duplicates";  // Папка дубликатов внутри устройства
                            fs::create_directories(duplicateDir);

                            fs::path newDuplicatePath = duplicateDir / (filePath.stem().string() + "_duplicated" +
                                std::to_string(duplicateCount) + filePath.extension().string());

                            fs::rename(filePath, getUniquePath(newDuplicatePath));

                            //std::lock_guard<std::mutex> coutLock(coutMutex);
                            // std::cout << "Дубликат перемещен: " << filePath << " -> " << newDuplicatePath << std::endl;
                        }
                        else {
                            // Не является дубликатом
                            fs::path newPath = newDir / filePath.filename();
                            fs::rename(filePath, getUniquePath(newPath));
                            hashMap[fileHash] = 1;

                            //std::lock_guard<std::mutex> coutLock(coutMutex);
                            // std::cout << "Перемещено: " << filePath << " -> " << newPath << std::endl;
                        }
                    }
                }
                else {
                    // Фото без Exif данных (скачанные с интернета, скриншоты, удалены данные)
                    fs::path noExifDataDir = targetDirectory / "NoExifData";
                    fs::create_directories(noExifDataDir);

                    {
                        std::lock_guard<std::mutex> lock(hashMapMutex);
                        if (hashMap.find(fileHash) != hashMap.end()) {
                            // Является дубликатом
                            int duplicateCount = hashMap[fileHash]++;
                            fs::path duplicateDir = noExifDataDir / "Duplicates";
                            fs::create_directories(duplicateDir);
                            fs::path newDuplicatePath = duplicateDir / (filePath.stem().string() + "_duplicated" +
                                std::to_string(duplicateCount) + filePath.extension().string());

                            fs::rename(filePath, getUniquePath(newDuplicatePath));

                            //std::lock_guard<std::mutex> coutLock(coutMutex);
                            // std::cout << "Дубликат без EXIF перемещен: " << filePath << " -> " << newDuplicatePath << std::endl;
                        }
                        else {
                            // Не является дубликатом
                            fs::path newPath = noExifDataDir / filePath.filename();
                            fs::rename(filePath, getUniquePath(newPath));
                            hashMap[fileHash] = 1;

                            //std::lock_guard<std::mutex> coutLock(coutMutex);
                            //std::cout << "Файл без EXIF данных перемещен: " << filePath << " -> " << newPath << std::endl;
                        }
                    }
                }
            }
            else {
                // Не является фотографией
                fs::path notPhotosDir = targetDirectory / "NotPhotos";
                fs::create_directories(notPhotosDir);

                {
                    std::lock_guard<std::mutex> lock(hashMapMutex);
                    if (hashMap.find(fileHash) != hashMap.end()) {
                        // Является дубликатом
                        int duplicateCount = hashMap[fileHash]++;
                        fs::path duplicateDir = notPhotosDir / "Duplicates";
                        fs::create_directories(duplicateDir);

                        fs::path newDuplicatePath = duplicateDir / (filePath.stem().string() + "_duplicated" +
                            std::to_string(duplicateCount) + filePath.extension().string());

                        fs::rename(filePath, getUniquePath(newDuplicatePath));

                        //std::lock_guard<std::mutex> coutLock(coutMutex);
                        //std::cout << "Дубликат не фото перемещен: " << filePath << " -> " << newDuplicatePath << std::endl;
                    }
                    else {
                        // Не является дубликатом
                        fs::path newPath = notPhotosDir / filePath.filename();
                        fs::rename(filePath, getUniquePath(newPath));
                        hashMap[fileHash] = 1;

                        //std::lock_guard<std::mutex> coutLock(coutMutex);
                        //std::cout << "Не изображение перемещено: " << filePath << " -> " << newPath << std::endl;
                    }
                }
            }
        }
        // Обработка ошибок
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
                processFilesWithDevice(chunk, targetDirectory, fileHashes);
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
                        //std::cout << "Перемещено: " << filePath << " -> " << newPath << std::endl;
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