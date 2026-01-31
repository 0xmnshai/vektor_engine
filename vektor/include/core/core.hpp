#pragma once

// Platform detection
#if defined(_WIN32)
    #define VEKTOR_PLATFORM_WINDOWS
#elif defined(__linux__)
    #define VEKTOR_PLATFORM_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
    #define VEKTOR_PLATFORM_MACOS
#else
    #error "Platform not supported!"
#endif

#if defined(VEKTOR_PLATFORM_WINDOWS)
    #if defined(VEKTOR_BUILD_DLL)
        #define VEKTOR_API __declspec(dllexport)
    #else
        #define VEKTOR_API __declspec(dllimport)
    #endif
#elif defined(VEKTOR_PLATFORM_LINUX) || defined(VEKTOR_PLATFORM_MACOS)
    #if defined(VEKTOR_BUILD_DLL)
        #define VEKTOR_API __attribute__((visibility("default")))
    #else
        #define VEKTOR_API
    #endif
#else
    #define VEKTOR_API
#endif

#define BIT(x) (1 << x)
 
#define VEKTOR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
