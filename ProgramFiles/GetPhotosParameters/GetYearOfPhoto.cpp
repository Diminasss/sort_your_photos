#include "GetYearOfPhoto.h"


std::string GetYearOfPhoto(const std::string& imagePath) {
    try {
        std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(imagePath);
        image->readMetadata();

        Exiv2::ExifData& exifData = image->exifData();
        if (exifData.empty()) {
            return "NoYearData";
        }

        // Проверяем наличие тега даты и времени съемки
        if (exifData.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal")) != exifData.end()) {
            std::string dateTimeOriginal = exifData["Exif.Photo.DateTimeOriginal"].toString();

            // Извлекаем год (формат "YYYY:MM:DD HH:MM:SS")
            if (dateTimeOriginal.length() >= 4) {
                return dateTimeOriginal.substr(0, 4); // Первые четыре символа — это год
            }
        }

        return "NoYearData";
    }
    catch (Exiv2::Error& e) {
        return "Error " + std::string(e.what());
    }
}