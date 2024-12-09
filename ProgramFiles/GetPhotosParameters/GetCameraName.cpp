#include "GetCameraName.h"
#include <exiv2/exiv2.hpp>


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

        // ���������� ������
        std::string MakedModel = make + " " + model;

        return MakedModel;
    }
    catch (Exiv2::Error& e) {
        return "Error " + std::string(e.what());
    }
}

//std::string GetCameraName(std::string& imagePath) {
//    try {
//        std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(imagePath);
//        image->readMetadata();
//
//        Exiv2::ExifData& exifData = image->exifData();
//        if (exifData.empty()) {
//            return "NoExifData";
//        }
//
//        std::string make = exifData["Exif.Image.Make"].toString();
//        std::string model = exifData["Exif.Image.Model"].toString();
//
//        // ����������� ��������� ������ ��� ������������ ������
//        std::string MakedModel;
//        MakedModel.reserve(make.size() + 1 + model.size()); // ����������� ������ ��� make + " " + model
//        MakedModel = std::move(make) + " " + std::move(model);
//
//        return MakedModel;
//    }
//    catch (Exiv2::Error& e) {
//        std::string error = e.what();
//        // cerr << "������ ��� ������ � ������������: " << error << endl;
//        return "Error " + error;
//    }
//}