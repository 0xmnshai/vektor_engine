#pragma once

#include "api.hpp"
#include "shared_library.hpp"

namespace vektor::plugin
{
    class PluginModule
    {
    public:
        explicit PluginModule(const std::string &path)
        {
            m_library.load(path);

            get_info_ = m_library.getSymbol<decltype(&vektor_get_plugin_info)>("vektor_get_plugin_info");
            create_ = m_library.getSymbol<decltype(&vektor_create_plugin)>("vektor_create_plugin");
            destroy_ = m_library.getSymbol<decltype(&vektor_destroy_plugin)>("vektor_destroy_plugin");

            m_info = get_info_();

            if (m_info.api_version != VEKTOR_PLUGIN_API_VERSION)
                throw std::runtime_error("Plugin API version mismatch");

            m_instance = create_();
            m_instance->on_load();
        }

        ~PluginModule()
        {
            if (m_instance)
            {
                m_instance->on_unload();
                destroy_(m_instance);
            }
        }

        const PluginInfo &info() const { return m_info; }

    private:
        SharedLibrary m_library;

        PluginInfo (*get_info_)();
        IPlugin *(*create_)();
        void (*destroy_)(IPlugin *);

        PluginInfo m_info;
        IPlugin *m_instance = nullptr;
    };
}