#include "getUniquePath.h"

fs::path getUniquePath(const fs::path& originalPath) {
    fs::path uniquePath = originalPath;
    int counter = 1;

    // Пока файл с таким именем существует, добавляем к имени суффикс с номером
    while (fs::exists(uniquePath)) {
        uniquePath = originalPath.parent_path() /
            (originalPath.stem().string() + "_" + std::to_string(counter) + originalPath.extension().string());
        counter++;
    }

    return uniquePath;
}