#include "GetCameraName.h"

// Функция для получения имени камеры, на которую была сделана фотография
std::string GetCameraName(const std::string& imagePath) {
    try {
        std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(imagePath);
        image->readMetadata();

        Exiv2::ExifData& exifData = image->exifData();
        if (exifData.empty()) {
            return "NoExifData";
        }

        std::string make = exifData["Exif.Image.Make"].toString();
        std::string model = exifData["Exif.Image.Model"].toString();

        std::string MakedModel = make + " " + model;

        return MakedModel;
    }
    catch (Exiv2::Error& e) {
        return "Error " + std::string(e.what());
    }
}