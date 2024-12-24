#include "SortByGadgetName.h"

// Функция с основной реализацией сортировки по устройству
void processFilesWithDevice(
    const std::vector<fs::path>& files,
    const fs::path& targetDirectory,
    std::unordered_map<std::string, int>& hashMap
) {
    writeLog("Функция запущена");
    std::mutex coutMutex;
    static std::mutex hashMapMutex; 

    for (const auto& filePath : files) {
        try {
            std::string fileHash = computeFileHash(filePath);

            if (isImageFile(filePath)) {
                // Является фотографией
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
                            fs::path duplicateDir = newDir / "Duplicates";  
                            fs::create_directories(duplicateDir);

                            fs::path newDuplicatePath = duplicateDir / (filePath.stem().string() + "_duplicated" +
                                std::to_string(duplicateCount) + filePath.extension().string());

                            fs::rename(filePath, getUniquePath(newDuplicatePath));
                        }
                        else {
                            // Не является дубликатом
                            fs::path newPath = newDir / filePath.filename();
                            fs::rename(filePath, getUniquePath(newPath));
                            hashMap[fileHash] = 1;
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
                        }
                        else {
                            // Не является дубликатом
                            fs::path newPath = noExifDataDir / filePath.filename();
                            fs::rename(filePath, getUniquePath(newPath));
                            hashMap[fileHash] = 1;
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
                    }
                    else {
                        // Не является дубликатом
                        fs::path newPath = notPhotosDir / filePath.filename();
                        fs::rename(filePath, getUniquePath(newPath));
                        hashMap[fileHash] = 1;
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

// Функуия для параллельной сортировки по устройству
void SortPhotosByGadgetNameParallel(const fs::path& directory, const fs::path& targetDirectory) {
    std::unordered_map<std::string, int> fileHashes; 

    std::vector<fs::path> allFiles;
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            allFiles.push_back(entry.path());
        }
    }

    size_t totalFiles = allFiles.size();
    size_t chunkSize = (totalFiles + 3) / 4;
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

// Функция на будущее для оптимизации
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