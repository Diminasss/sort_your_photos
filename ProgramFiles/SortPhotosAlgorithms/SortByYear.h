#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <iostream>
#include <thread>
#include <filesystem>

#include "../CommonFunctions/computeFileHash.h"
#include "../CommonFunctions/IsImageFile.h"
#include "../CommonFunctions/getUniquePath.h"
#include "../CommonFunctions/SanitizeFolderName.h"

#include "../GetPhotosParameters/GetYearOfPhoto.h"

namespace fs = std::filesystem;

void SortByYearParallel(const fs::path& directory, const fs::path& targetDirectory);