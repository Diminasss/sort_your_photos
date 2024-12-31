#include "SortByGadgetName.h"

// Функция с основной реализацией сортировки по устройству
void processFilesWithDevice(
    const std::vector<fs::path>& files,
    const fs::path& targetDirectory,
    std::unordered_map<std::string, int>& hashMap,
    QWidget* parent
) {
    writeLog("Путь получен в processFileWithDevice");
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
                    writeLog("Путь использован для создания реального каталога");
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
            if (parent) {
                QMetaObject::invokeMethod(parent, [parent, filePath, e]() {
                    QMessageBox::warning(
                        parent,
                        "Ошибка обработки файла",
                        QString("Ошибка: %1\nФайл: %2")
                        .arg(e.what())
                        .arg(QString::fromStdString(filePath.string()))
                    );
                    });
            }
        }
    }
}

// Функуия для параллельной сортировки по устройству
void SortPhotosByGadgetNameParallel(const fs::path& directory, const fs::path& targetDirectory, QWidget* parent) {
    writeLog("Путь получен в паралеле");
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
            writeLog("Путь передаётся в запуск потоков");
            // Запускаем поток с помощью лямбда-функции
            threads.emplace_back([chunk, &targetDirectory, &fileHashes, parent]() {
                processFilesWithDevice(chunk, targetDirectory, fileHashes, parent);
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