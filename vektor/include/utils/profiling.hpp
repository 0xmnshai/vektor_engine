#pragma once
#include <string>
#include <cstdint>

namespace vektor::utils
{
    struct ProfileResult
    {
        std::string name;
        long long start;
        long long end;
        uint32_t threadID;
    };
}
