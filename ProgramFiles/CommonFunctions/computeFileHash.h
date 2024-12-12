#pragma once
#include <string>
#include <filesystem>
#include <openssl/evp.h>
#include <fstream>

namespace fs = std::filesystem;

std::string computeFileHash(const fs::path& filePath);