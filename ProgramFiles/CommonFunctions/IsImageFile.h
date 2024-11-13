#pragma once
#include <filesystem>

namespace fs = std::filesystem;
bool isImageFile(const fs::path& filePath);