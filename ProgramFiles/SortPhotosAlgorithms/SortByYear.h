#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <iostream>
#include <thread>
#include <filesystem>

namespace fs = std::filesystem;

void SortByYearParallel(const fs::path& directory, const fs::path& targetDirectory);