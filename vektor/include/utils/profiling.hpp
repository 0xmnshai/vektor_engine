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
        std::__thread_id threadID;
    };
}
