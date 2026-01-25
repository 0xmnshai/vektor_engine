#pragma once

#include "vk_pch.hpp"
#include "core/core.hpp"

namespace vektor::logger
{
    class VEKTOR_API Logger
    {
    public:
        static void Init();
        static void Shutdown();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }

    private:
        // These are declarations. Definitions go in .cpp
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;
    };
}
 
#define VEKTOR_CORE_TRACE(...)    ::vektor::logger::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define VEKTOR_CORE_INFO(...)     ::vektor::logger::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define VEKTOR_CORE_WARN(...)     ::vektor::logger::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define VEKTOR_CORE_ERROR(...)    ::vektor::logger::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define VEKTOR_CORE_CRITICAL(...) ::vektor::logger::Logger::GetCoreLogger()->critical(__VA_ARGS__)
 
#define VEKTOR_TRACE(...)         ::vektor::logger::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define VEKTOR_INFO(...)          ::vektor::logger::Logger::GetClientLogger()->info(__VA_ARGS__)
#define VEKTOR_WARN(...)          ::vektor::logger::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define VEKTOR_ERROR(...)         ::vektor::logger::Logger::GetClientLogger()->error(__VA_ARGS__)
#define VEKTOR_CRITICAL(...)      ::vektor::logger::Logger::GetClientLogger()->critical(__VA_ARGS__)
 
#ifdef VEKTOR_ENABLE_ASSERTS
    #if defined(_MSC_VER)
        #define VEKTOR_DEBUGBREAK() __debugbreak()
    #else
        #define VEKTOR_DEBUGBREAK() __builtin_trap()
    #endif

    #define VEKTOR_CORE_ASSERT(x, ...) { if(!(x)) { VEKTOR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); VEKTOR_DEBUGBREAK(); } }
    #define VEKTOR_ASSERT(x, ...)      { if(!(x)) { VEKTOR_ERROR("Assertion Failed: {0}", __VA_ARGS__); VEKTOR_DEBUGBREAK(); } }
#else
    #define VEKTOR_CORE_ASSERT(x, ...)
    #define VEKTOR_ASSERT(x, ...)
#endif