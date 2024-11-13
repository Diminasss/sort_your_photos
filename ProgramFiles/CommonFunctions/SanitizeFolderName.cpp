#include "SanitizeFolderName.h"

// ������� ��� �������� ������������ �������� �� ����� �����
std::string sanitizeFolderName(const std::string& name) {
    std::string sanitized = name;
    // ������ ��������, ������� ������ ������������ � ������ ������/����� � Windows
    std::string invalidChars = "\\/:*?\"<>|";

    // �������� ��� ������������ ������� �� �������������
    for (char& c : sanitized) {
        if (invalidChars.find(c) != std::string::npos) {
            c = '_';  // �������� ������������ ������ �� _
        }
    }

    return sanitized;
}