#include "SortByGadgetName.h"

#include "../CommonFunctions/IsImageFile.h"
#include "../CommonFunctions/SanitizeFolderName.h"
#include "../GetPhotosParameters/GetCameraName.h"

#include <iostream>


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