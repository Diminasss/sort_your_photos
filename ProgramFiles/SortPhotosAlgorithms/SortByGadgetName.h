#pragma once
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

void SortPhotosByGadgetName(const fs::path& directory, const fs::path& targetDirectory);
void processFiles(const std::vector<fs::path>& files, const fs::path& targetDirectory);
void SortPhotosByGadgetNameParallel(const fs::path& directory, const fs::path& targetDirectory);