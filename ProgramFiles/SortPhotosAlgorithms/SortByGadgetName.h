#pragma once
#include <filesystem>

namespace fs = std::filesystem;

void SortPhotosByGadgetName(const fs::path& directory, const fs::path& targetDirectory);