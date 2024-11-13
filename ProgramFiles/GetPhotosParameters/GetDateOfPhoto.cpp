#include "GetDateOfPhoto.h"
#include <exiv2/exiv2.hpp>

std::string GetDateOfPhoto(const std::string& filePath) {
    try {
        // Открываем изображение
        std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filePath);
        if (!image) {
            throw std::runtime_error("Не удалось открыть изображение.");
        }

        // Читаем метаданные
        image->readMetadata();
        Exiv2::ExifData& exifData = image->exifData();

        // Проверяем, есть ли в метаданных дата съемки
        if (exifData.empty()) {
            return "NoExifData";
        }

        auto pos = exifData.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal"));
        if (pos != exifData.end()) {
            return pos->value().toString();
        }
        else {
            return "NoDateInExifData";
        }
    }
    catch (Exiv2::Error& e) {
        std::string error = e.what();
        // cerr << "Ошибка при работе с изображением: " << error << endl;
        return "Error " + error;
    }
}