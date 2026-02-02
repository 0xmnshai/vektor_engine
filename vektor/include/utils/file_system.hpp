
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

#include "core/core.hpp"

namespace vektor::utils
{
    class VEKTOR_API FileSystem
    {
    public:
        FileSystem() = delete;

        static std::string readFileToString(const std::filesystem::path &filepath);

        static std::vector<uint8_t> readFileToBytes(const std::filesystem::path &filepath);

        static bool exists(const std::filesystem::path &filepath);

        static std::filesystem::path getAbsolutePath(const std::filesystem::path &filepath);
    };
}