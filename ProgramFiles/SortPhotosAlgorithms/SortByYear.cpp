﻿#include "SortByYear.h"

// Основная реализация сортировки по году
void processFilesWithYear(
    const std::vector<fs::path>& files,
    const fs::path& targetDirectory,
    std::unordered_map<std::string, int>& hashMap
) {
    std::mutex coutMutex;
    static std::mutex hashMapMutex;

    for (const auto& filePath : files) {
        try {
            // Считаем хеш файла
            std::string fileHash = computeFileHash(filePath);

            if (isImageFile(filePath)) {
                // Является картинкой
                std::string year = GetYearOfPhoto(filePath.string());

                if (year != "NoYearData" && year.find("Error") == std::string::npos) {
                    // Фото имеет данные о годе
                    std::string sanitizedYear = sanitizeFolderName(year); // Санитизация имени директории
                    fs::path yearDir = targetDirectory / sanitizedYear;
                    fs::create_directories(yearDir);

                    {
                        std::lock_guard<std::mutex> lock(hashMapMutex);
                        if (hashMap.find(fileHash) != hashMap.end()) {
                            // Является дубликатом
                            int duplicateCount = hashMap[fileHash]++;
                            fs::path duplicateDir = yearDir / "Duplicates";  // Папка дубликатов внутри года
                            fs::create_directories(duplicateDir);

                            fs::path newDuplicatePath = duplicateDir / (filePath.stem().string() + "_duplicated" +
                                std::to_string(duplicateCount) + filePath.extension().string());

                            fs::rename(filePath, getUniquePath(newDuplicatePath));
                        }
                        else {
                            // Не является дубликатом
                            fs::path newPath = yearDir / filePath.filename();
                            fs::rename(filePath, getUniquePath(newPath));
                            hashMap[fileHash] = 1;
                        }
                    }
                }
                else {
                    // Фото без данных о годе
                    fs::path noYearDir = targetDirectory / "NoYearData";
                    fs::create_directories(noYearDir);

                    {
                        std::lock_guard<std::mutex> lock(hashMapMutex);
                        if (hashMap.find(fileHash) != hashMap.end()) {
                            // Является дубликатом
                            int duplicateCount = hashMap[fileHash]++;
                            fs::path duplicateDir = noYearDir / "Duplicates";
                            fs::create_directories(duplicateDir);

                            fs::path newDuplicatePath = duplicateDir / (filePath.stem().string() + "_duplicated" +
                                std::to_string(duplicateCount) + filePath.extension().string());

                            fs::rename(filePath, getUniquePath(newDuplicatePath));
                        }
                        else {
                            // Не является дубликатом
                            fs::path newPath = noYearDir / filePath.filename();
                            fs::rename(filePath, getUniquePath(newPath));
                            hashMap[fileHash] = 1;
                        }
                    }
                }
            }
            else {
                // Не является картинкой
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


// Функция для сортировки по году параллельно
void SortByYearParallel(const fs::path& directory, const fs::path& targetDirectory) {
    std::unordered_map<std::string, int> fileHashes;

    std::vector<fs::path> allFiles;
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            allFiles.push_back(entry.path());
        }
    }

    size_t totalFiles = allFiles.size();
    size_t chunkSize = (totalFiles + 3) / 4; // 4 потока
    std::vector<std::thread> threads;

    for (size_t i = 0; i < 4; ++i) {
        size_t startIdx = i * chunkSize;
        size_t endIdx = std::min(startIdx + chunkSize, totalFiles);

        if (startIdx < totalFiles) {
            std::vector<fs::path> chunk(allFiles.begin() + startIdx, allFiles.begin() + endIdx);

            threads.emplace_back([chunk, &targetDirectory, &fileHashes]() {
                processFilesWithYear(chunk, targetDirectory, fileHashes);
                });
        }
    }
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
