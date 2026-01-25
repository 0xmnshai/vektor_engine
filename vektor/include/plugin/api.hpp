#pragma once

#include "vk_pch.hpp"
#include "shared_library.hpp"

namespace vektor::plugin
{
    struct PluginInfo
    {
        const std::string *name;
        const char *version;
        uint32_t api_version;
    };

    constexpr uint32_t VEKTOR_PLUGIN_API_VERSION = 1;

    class IPlugin
    {
    public:
        virtual ~IPlugin() = default;
        virtual void on_load() = 0;
        virtual void on_unload() = 0;

        // virtual void register_components(vektor::ECS &) = 0;
        // virtual void register_systems(vektor::ECS &) = 0;
        // virtual void update(vektor::ECS &, float dt) = 0;
    };

    // C ABI — never change names or signatures
    extern "C"
    {
        VEKTOR_PLUGIN_API PluginInfo vektor_get_plugin_info();
        VEKTOR_PLUGIN_API IPlugin *vektor_create_plugin();
        VEKTOR_PLUGIN_API void vektor_destroy_plugin(IPlugin *);
    }
}
