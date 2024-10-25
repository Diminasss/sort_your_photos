#include <iostream>
#include <string>
#include <vector>
#include <exiv2/exiv2.hpp>
#include <windows.h>

using namespace std;

string GetCameraName(string imagePath) {
    try {
        unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(imagePath);
        image->readMetadata();

        Exiv2::ExifData& exifData = image->exifData();
        if (exifData.empty()) {
            return "NoExifData";
        }

        string make = exifData["Exif.Image.Make"].toString();
        string model = exifData["Exif.Image.Model"].toString();
        string MakedModel = make + " " + model;
        // cout << MakedModel << endl;
        return MakedModel;

    }
    catch (Exiv2::Error& e) {
        string error = e.what();
        // cerr << "Ошибка при работе с изображением: " << error << endl;
        string error_return = "Error " + error;
        return error_return;
    }
}
int main() {
    
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    vector<string> paths = { "C:\\Users\\johnworker\\Desktop\\Проекты C++\\Sort your photos\\TestPhotos\\Screenshot.png", "C:\\Users\\johnworker\\Desktop\\Проекты C++\\Sort your photos\\TestPhotos\\IMG_20240227_155733.jpg", "C:\\Users\\johnworker\\Desktop\\Проекты C++\\Sort your photos\\TestPhotos\\IMG_20240227_201904.jpg", "C:\\Users\\johnworker\\Desktop\\Проекты C++\\Sort your photos\\TestPhotos\\IMG_20241022_185408.jpg", "C:\\Users", "C:\\Users\\johnworker\\Desktop\\Проекты C++\\Sort your photos\\TestPhotos\\20230204_171234.dng", "C:\\Users\\johnworker\\Desktop\\Проекты C++\\Sort your photos\\TestPhotos\\Лаб2.xlsx" };
    for (string x: paths) {
        cout << GetCameraName(x) << endl;
    }
    

    return 0;
}

