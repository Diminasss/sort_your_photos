#include "frontend_sort_photo/mainwindow.h"
#include "ProgramFiles/SortPhotosAlgorithms/SortByGadgetName.h"
#include "ProgramFiles/SortPhotosAlgorithms/SortByYear.h"

#include "ProgramFiles/CommonFunctions/writeLog.h"
#include "ProgramFiles/CommonFunctions/clearAndOpen.h"

#include <QApplication>

#define writeLog(msg) writeLog(msg, __FILE__, __LINE__)

//#include <filesystem>
// На данный момент требуется сделать проверку работы функции параллельной сортировки и понять, что с ней не так

namespace fs = std::filesystem;


int main(int argc, char* argv[]) {
    clearLogFileAndOpenNotepad();

    // Указываем начальную директорию, где искать фото
    //const fs::path sourceDirectory = "C:\\Users\\johnworker\\Desktop\\Проекты C++\\sort your photos qt\\TestPhotos";
    //writeLog("Инициализирован исходный путь");
    // Указываем папку для сортировки (куда будут перемещаться фото)
    //const fs::path targetDirectory = "C:\\Users\\johnworker\\Desktop\\Проекты C++\\sort your photos qt\\SortedPhotos";
    //writeLog("Инициализирован второй путь");
    //SortPhotosByGadgetNameParallel(sourceDirectory, targetDirectory);
    //writeLog("Произведена сортировка");


    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}



//#include <iostream>
//#include <chrono>
//#include <string>
//#include <windows.h>
//#include <filesystem>
//#include <thread>
//
//
//#include "ProgramFiles/SortPhotosAlgorithms/SortByGadgetName.h"
//#include "ProgramFiles/SortPhotosAlgorithms/SortByYear.h"
//
//
//using namespace std;
//namespace fs = std::filesystem;
//
//int CountProcessorCores() {
//    // unsigned - может хранить только неотрицательные числа
//    unsigned int numThreads = std::thread::hardware_concurrency(); 
//    return numThreads;
//}
//
//int main() {
//    cout << CountProcessorCores();
//    SetConsoleOutputCP(1251);
//    SetConsoleCP(1251);
//
//
//    // Указываем начальную директорию, где искать фото
//    const fs::path sourceDirectory = "C:\\Users\\johnworker\\Desktop\\Проекты C++\\sort your photos qt\\TestPhotos";
//
//    // Указываем папку для сортировки (куда будут перемещаться фото)
//    const fs::path targetDirectory = "C:\\Users\\johnworker\\Desktop\\Проекты C++\\sort your photos qt\\SortedPhotos";
//
//    // Запуск обработки файлов
//    //SortPhotosByGadgetName(sourceDirectory, targetDirectory);
//    auto startTime = std::chrono::high_resolution_clock::now();
//    // SortByYearParallel(sourceDirectory, targetDirectory);
//    SortPhotosByGadgetNameParallel(sourceDirectory, targetDirectory);
//    auto endTime = std::chrono::high_resolution_clock::now();
//
//    // Вычисляем разницу во времени в секундах
//    std::chrono::duration<double> elapsedSeconds = endTime - startTime;
//    std::cout << "Время выполнения: " << elapsedSeconds.count() << " секунд" << std::endl;
//
//    return 0;
//}