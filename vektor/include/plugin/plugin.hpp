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

// Shared library extension
#if defined(VEKTOR_PLATFORM_WINDOWS)
    #define VEKTOR_PLUGIN_EXTENSION ".dll"
#elif defined(VEKTOR_PLATFORM_MACOS)
    #define VEKTOR_PLUGIN_EXTENSION ".dylib"
#elif defined(VEKTOR_PLATFORM_LINUX)
    #define VEKTOR_PLUGIN_EXTENSION ".so"
#endif

// Export / import
#if defined(VEKTOR_PLATFORM_WINDOWS)
    #if defined(VEKTOR_BUILD_PLUGIN)
        #define VEKTOR_PLUGIN_API __declspec(dllexport)
    #else
        #define VEKTOR_PLUGIN_API __declspec(dllimport)
    #endif
#else
    #if defined(VEKTOR_BUILD_PLUGIN)
        #define VEKTOR_PLUGIN_API __attribute__((visibility("default")))
    #else
        #define VEKTOR_PLUGIN_API
    #endif
#endif
