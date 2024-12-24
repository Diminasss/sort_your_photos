#pragma once
#include <iostream>
#include <filesystem>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <thread>
#include <string>
#include <algorithm>

#include "../GetPhotosParameters/GetCameraName.h"

#include "../CommonFunctions/IsImageFile.h"
#include "../CommonFunctions/SanitizeFolderName.h"
#include "../CommonFunctions/computeFileHash.h"
#include "../CommonFunctions/getUniquePath.h"

#include "../CommonFunctions/writeLog.h"
#define writeLog(msg) writeLog(msg, __FILE__, __LINE__)

namespace fs = std::filesystem;

void SortPhotosByGadgetName(const fs::path& directory, const fs::path& targetDirectory);
void SortPhotosByGadgetNameParallel(const fs::path& directory, const fs::path& targetDirectory);