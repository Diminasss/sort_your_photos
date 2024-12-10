#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>

namespace fs = std::filesystem;
bool isImageFile(const fs::path& filePath);