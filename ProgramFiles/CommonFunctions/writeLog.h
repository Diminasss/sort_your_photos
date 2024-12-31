#pragma once
#include <QString>
#include <QDebug>
#include <QLoggingCategory>

#include <fstream>
#include <filesystem>


namespace fs = std::filesystem;

//void writeLog(const QString& message);
void writeLog(const QString& message, const char* file, int line);