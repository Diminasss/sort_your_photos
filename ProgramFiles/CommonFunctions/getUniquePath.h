#pragma once
#include <filesystem>

namespace fs = std::filesystem;

fs::path getUniquePath(const fs::path& originalPath);