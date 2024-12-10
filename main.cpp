#include <iostream>
#include <chrono>
#include <string>
#include <windows.h>
#include <filesystem>
#include <thread>

#include "ProgramFiles/SortPhotosAlgorithms/SortByGadgetName.h"
#include "ProgramFiles/SortPhotosAlgorithms/SortByYear.h"

using namespace std;
namespace fs = std::filesystem;

int CountProcessorCores() {
    // unsigned - может хранить только неотрицательные числа
    unsigned int numThreads = std::thread::hardware_concurrency(); 
    return numThreads;
}

int main() {
    cout << CountProcessorCores();
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);


    vector<string> paths = { "C:\\Users\\johnworker\\Desktop\\Проекты C++\\Sort your photos\\TestPhotos\\Screenshot.png", "C:\\Users\\johnworker\\Desktop\\Проекты C++\\Sort your photos\\TestPhotos\\IMG_20240227_155733.jpg", "C:\\Users\\johnworker\\Desktop\\Проекты C++\\Sort your photos\\TestPhotos\\IMG_20240227_201904.jpg", "C:\\Users\\johnworker\\Desktop\\Проекты C++\\Sort your photos\\TestPhotos\\IMG_20241022_185408.jpg", "C:\\Users", "C:\\Users\\johnworker\\Desktop\\Проекты C++\\Sort your photos\\TestPhotos\\20230204_171234.dng", "C:\\Users\\johnworker\\Desktop\\Проекты C++\\Sort your photos\\TestPhotos\\Лаб2.xlsx" };
    // Указываем начальную директорию, где искать фото
    const fs::path sourceDirectory = "C:\\Users\\johnworker\\Desktop\\Проекты C++\\Sort your photos\\TestPhotos";

    // Указываем папку для сортировки (куда будут перемещаться фото)
    const fs::path targetDirectory = "C:\\Users\\johnworker\\Desktop\\Проекты C++\\Sort your photos\\SortedPhotos";

    // Запуск обработки файлов
    //SortPhotosByGadgetName(sourceDirectory, targetDirectory);
    auto startTime = std::chrono::high_resolution_clock::now();
    // SortByYearParallel(sourceDirectory, targetDirectory);
    SortPhotosByGadgetNameParallel(sourceDirectory, targetDirectory);
    auto endTime = std::chrono::high_resolution_clock::now();

    // Вычисляем разницу во времени в секундах
    std::chrono::duration<double> elapsedSeconds = endTime - startTime;
    std::cout << "Время выполнения: " << elapsedSeconds.count() << " секунд" << std::endl;

    return 0;
}

