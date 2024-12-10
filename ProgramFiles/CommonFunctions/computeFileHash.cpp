#include "computeFileHash.h"


std::string computeFileHash(const fs::path& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        return "";
    }

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create OpenSSL context.");
    }

    const EVP_MD* md = EVP_sha256();
    if (!md) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize SHA256.");
    }

    if (EVP_DigestInit_ex(ctx, md, nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize Digest.");
    }

    char buffer[8192];
    while (file.read(buffer, sizeof(buffer))) {
        if (EVP_DigestUpdate(ctx, buffer, file.gcount()) != 1) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("Failed to update Digest.");
        }
    }
    if (file.gcount() > 0) {
        if (EVP_DigestUpdate(ctx, buffer, file.gcount()) != 1) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("Failed to update Digest.");
        }
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLength = 0;
    if (EVP_DigestFinal_ex(ctx, hash, &hashLength) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize Digest.");
    }

    EVP_MD_CTX_free(ctx);

    std::stringstream hashStream;
    for (unsigned int i = 0; i < hashLength; i++) {
        hashStream << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return hashStream.str();
}