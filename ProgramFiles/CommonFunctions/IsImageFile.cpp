#include "IsImageFile.h"

bool isImageFile(const fs::path& filePath) {
    // ��� ��������� �������� �������, ��� ����������� ����� ���������� .jpg, .jpeg, .png, .bmp � �. �.
    std::vector<std::string> imageExtensions = { ".jpg", ".jpeg", ".png", ".bmp", ".tiff", ".gif", ".dng" };

    std::string extension = filePath.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower); // �������� ���������� � ������� ��������

    return std::find(imageExtensions.begin(), imageExtensions.end(), extension) != imageExtensions.end();
}