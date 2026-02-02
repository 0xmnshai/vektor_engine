#include "vk_pch.hpp"
#include "utils/file_system.hpp"
#include "logger/logger.hpp"

namespace vektor::utils
{
    std::string FileSystem::readFileToString(const std::filesystem::path &filepath)
    {
        if (!exists(filepath))
        {
            VEKTOR_CORE_ERROR("File does not exist: {0}", filepath.string());
            return "";
        }

        std::ifstream file(filepath, std::ios::in | std::ios::binary);

        if (!file)
        {
            VEKTOR_CORE_ERROR("Could not open file: {0}", filepath.string());
            return "";
        }

        std::string result;
        uintmax_t size = std::filesystem::file_size(filepath);

        if (size > 0)
        {
            result.resize(size);
            file.read(result.data(), size);
        }

        return result;
    }

    std::vector<uint8_t> FileSystem::readFileToBytes(const std::filesystem::path &filepath)
    {
        if (!exists(filepath))
        {
            VEKTOR_CORE_ERROR("File does not exist: {0}", filepath.string());
            return {};
        }

        std::ifstream file(filepath, std::ios::binary);
        uintmax_t size = std::filesystem::file_size(filepath);

        std::vector<uint8_t> buffer(size);
        file.read(reinterpret_cast<char *>(buffer.data()), size);

        return buffer;
    }

    bool FileSystem::exists(const std::filesystem::path &filepath)
    {
        return std::filesystem::exists(filepath);
    }

    std::filesystem::path FileSystem::getAbsolutePath(const std::filesystem::path &filepath)
    {
        return std::filesystem::absolute(filepath);
    }
}