#pragma once
#include <iostream>
#include <filesystem>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <thread>
#include <string>
#include <algorithm>


namespace fs = std::filesystem;

void SortPhotosByGadgetName(const fs::path& directory, const fs::path& targetDirectory);
void SortPhotosByGadgetNameParallel(const fs::path& directory, const fs::path& targetDirectory);