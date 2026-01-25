#pragma once
 
#include "core/core.hpp"
#include "plugin/plugin.hpp"

#if defined(VEKTOR_PLATFORM_WINDOWS)
#include <windows.h>
using SharedLibHandle = HMODULE;
#else
#include <dlfcn.h>
using SharedLibHandle = void *;
#endif

namespace vektor::plugin
{
    class SharedLibrary
    {
    public:
        SharedLibrary() noexcept : m_handle(nullptr) {}
        explicit SharedLibrary(const std::string &path) { load(path); }

        ~SharedLibrary() { unload(); }

        // non-copyable
        SharedLibrary(const SharedLibrary &) = delete;
        SharedLibrary &operator=(const SharedLibrary &) = delete;

        // movable
        SharedLibrary(SharedLibrary &&other) noexcept : m_handle(other.m_handle) { other.m_handle = nullptr; }
        SharedLibrary &operator=(SharedLibrary &&other) noexcept
        {
            if (this != &other)
            {
                unload();
                m_handle = other.m_handle;
                other.m_handle = nullptr;
            }
            return *this;
        }

        void load(const std::string &path)
        {

            unload();
#if defined(VEKTOR_PLATFORM_WINDOWS)
            m_handle = LoadLibraryA(path.c_str());
            if (!m_handle)
            {
                throw std::system_error("Failed to load shared library: " + path, GetLastError(), std::system_category());
            }
#else
            m_handle = dlopen(path.c_str(), RTLD_LAZY);
            if (!m_handle)
            {
                throw std::runtime_error("Failed to load shared library: " + path + " Error: " + dlerror());
            }
#endif
        }

        void unload() noexcept
        {

            if (!m_handle)
                return;

#if defined(VEKTOR_PLATFORM_WINDOWS)
            FreeLibrary(static_cast<HMODULE>(m_handle));
#else
            dlclose(m_handle);
#endif
            m_handle = nullptr;
        }

        [[nodiscard]] bool isLoaded() const noexcept { return m_handle != nullptr; }

        template <typename T>
        T getSymbol(const std::string &name) const
        {
            if (!m_handle)
            {
                throw std::runtime_error("Shared library not loaded");
            }

#if defined(VEKTOR_PLATFORM_WINDOWS)
            return reinterpret_cast<T>(GetProcAddress(static_cast<HMODULE>(m_handle), name.c_str()));
#else
            return reinterpret_cast<T>(dlsym(m_handle, name.c_str()));
#endif
        }

    private:
        void *m_handle;
    };
}
