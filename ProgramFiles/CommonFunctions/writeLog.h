#pragma once
#include <QString>
#include <QDebug>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

//void writeLog(const QString& message);
void writeLog(const QString& message, const char* file, int line);