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

// ������� ��� ������������� ������ (���� ����� ���������� ������ �������)
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
    static std::mutex hashMapMutex; // ������� ��� ������ hashMap

    for (const auto& filePath : files) {
        try {
            if (isImageFile(filePath)) {
                std::string cameraName = GetCameraName(filePath.string());

                if (cameraName != "NoExifData" && cameraName != "Error") {
                    std::string sanitizedCameraName = sanitizeFolderName(cameraName);

                    fs::path newDir = targetDirectory / sanitizedCameraName;
                    fs::create_directories(newDir);

                    // ������� ��� �����
                    std::string fileHash = computeFileHash(filePath);

                    {
                        std::lock_guard<std::mutex> lock(hashMapMutex);
                        // ���������, ���� �� ��� ����� ���� � ����� �����
                        if (hashMap.find(fileHash) != hashMap.end()) {
                            int duplicateCount = hashMap[fileHash]++;
                            fs::path duplicateDir = targetDirectory / "Duplicates";

                            // ������ ����� ��� ����������, ���� � ���
                            fs::create_directories(duplicateDir);

                            fs::path newDuplicatePath = duplicateDir / (sanitizedCameraName + "_duplicate" +
                                std::to_string(duplicateCount) + filePath.extension().string());
                            fs::rename(filePath, newDuplicatePath);

                            std::lock_guard<std::mutex> coutLock(coutMutex);
                            std::cout << "�������� ���������: " << filePath << " -> " << newDuplicatePath << std::endl << std::endl;
                        }
                        else {
                            // ���� ���� �� ��������, ���������� ��� � ����� �� ����� ������
                            fs::path newPath = newDir / filePath.filename();
                            fs::rename(filePath, newPath);

                            // ��������� ��� � �����
                            hashMap[fileHash] = 1;

                            std::lock_guard<std::mutex> coutLock(coutMutex);
                            std::cout << "����������: " << filePath << " -> " << newPath << std::endl << std::endl;
                        }
                    }
                }
                else {
                    fs::path noExifDataDir = targetDirectory / "NoExifData";
                    fs::create_directories(noExifDataDir);
                    fs::path newPath = noExifDataDir / filePath.filename();
                    fs::rename(filePath, newPath);

                    std::lock_guard<std::mutex> coutLock(coutMutex);
                    std::cout << "���� ��� EXIF ������ ���������: " << filePath << " -> " << newPath << std::endl << std::endl;
                }
            }
            else {
                fs::path notPhotosDir = targetDirectory / "NotPhotos";
                fs::create_directories(notPhotosDir);
                fs::path newPath = notPhotosDir / filePath.filename();
                fs::rename(filePath, newPath);

                std::lock_guard<std::mutex> coutLock(coutMutex);
                std::cout << "�� ����������� ����������: " << filePath << " -> " << newPath << std::endl << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cerr << "������: " << e.what() << " ��� ����� " << filePath << std::endl;
        }
    }
}


void SortPhotosByGadgetNameParallel(const fs::path& directory, const fs::path& targetDirectory) {
    std::unordered_map<std::string, int> fileHashes; // ����� ��� �������� ���-����

    std::vector<fs::path> allFiles;
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            allFiles.push_back(entry.path());
        }
    }

    size_t totalFiles = allFiles.size();
    size_t chunkSize = (totalFiles + 3) / 4; // ��������� ����� �� 4 �����
    std::vector<std::thread> threads;

    for (size_t i = 0; i < 4; ++i) {
        size_t startIdx = i * chunkSize;
        size_t endIdx = std::min(startIdx + chunkSize, totalFiles);

        if (startIdx < totalFiles) {
            // ��������� ����� ������ ��� �������� ������
            std::vector<fs::path> chunk(allFiles.begin() + startIdx, allFiles.begin() + endIdx);

            // ��������� ����� � ������� ������-�������
            threads.emplace_back([chunk, &targetDirectory, &fileHashes]() {
                processFiles(chunk, targetDirectory, fileHashes);
                });
        }
    }

    // ������� ���������� ���� �������
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}





//// ������� ��� ��������� ����� ������ ������� ��� ��������� ����
//void processFiles(const std::vector<fs::path>& files, const fs::path& targetDirectory) {
//    for (const auto& filePath : files) {
//        try {
//            // ���������, �������� �� ���� ������������
//            if (isImageFile(filePath)) {
//                // �������� ��� ������ ��� ����������
//                std::string cameraName = GetCameraName(filePath.string());
//
//                if (cameraName != "NoExifData" && cameraName != "Error") {
//                    std::string sanitizedCameraName = sanitizeFolderName(cameraName);
//
//                    fs::path newDir = targetDirectory / sanitizedCameraName;  // ����� � ������ ������
//                    fs::create_directories(newDir);  // ������� �����, ���� � ���
//
//                    fs::path newPath = newDir / filePath.filename();
//                    fs::rename(filePath, newPath);
//
//                    std::lock_guard<std::mutex> lock(coutMutex);
//                    // std::cout << "����������: " << filePath << " -> " << newPath << std::endl;
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
//            std::cerr << "������: " << e.what() << " ��� ����� " << filePath << std::endl;
//        }
//    }
//}

//void SortPhotosByGadgetNameParallel(const fs::path& directory, const fs::path& targetDirectory) {
//    std::vector<fs::path> allFiles;
//
//    // ��������� ��� ����� � ����������
//    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
//        if (entry.is_regular_file()) {
//            allFiles.push_back(entry.path());
//        }
//    }
//
//    // ��������� ����� �� 4 �����
//    size_t totalFiles = allFiles.size();
//    size_t chunkSize = (totalFiles + 3) / 4;  // ��������� � ������� �������
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
//    // ������� ���������� ���� �������
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
                SortPhotosByGadgetName(entry.path(), targetDirectory);  // ����������� ����� ��� ������������
            }
            else if (entry.is_regular_file()) {
                std::string filePath = entry.path().string();

                // ���������, �������� �� ���� ������������
                if (isImageFile(entry.path())) {
                    // �������� ��� ������ ��� ����������
                    std::string cameraName = GetCameraName(filePath);

                    if (cameraName != "NoExifData" && cameraName != "Error") {
                        // ������������ ��� ������, ����� ������� ��� ���������� ��� ����� �����
                        std::string sanitizedCameraName = sanitizeFolderName(cameraName);

                        fs::path newDir = targetDirectory / sanitizedCameraName;  // ����� � ������ ������
                        fs::create_directories(newDir);  // ������� �����, ���� � ���

                        // ��������� ����� ���� ��� �����������
                        fs::path newPath = newDir / entry.path().filename();

                        // ���������� ����
                        fs::rename(entry.path(), newPath);
                        std::cout << "����������: " << filePath << " -> " << newPath << std::endl;
                    }
                    else {
                        // �������� ������, ���� EXIF-������ �����������
                        std::cerr << "������ ��������� ����������� (No EXIF data): " << filePath << std::endl;

                        // ���������� � ����� NoExifData
                        fs::path noExifDataDir = targetDirectory / "NoExifData";
                        fs::create_directories(noExifDataDir);
                        fs::path newPath = noExifDataDir / entry.path().filename();
                        fs::rename(entry.path(), newPath);
                    }
                }
                else {
                    // �������� ������, ���� ���� �� �������� ������������
                    std::cerr << "�� �������� ������������: " << filePath << std::endl;

                    // ���������� � ����� NotPhotos
                    fs::path notPhotosDir = targetDirectory / "NotPhotos";
                    fs::create_directories(notPhotosDir);
                    fs::path newPath = notPhotosDir / entry.path().filename();
                    fs::rename(entry.path(), newPath);
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "������: " << e.what() << std::endl;
    }
}
