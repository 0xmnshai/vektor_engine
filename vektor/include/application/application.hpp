#pragma once

#include "core/core.hpp"
#include "window/window.hpp"

namespace vektor
{
    class VEKTOR_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

    private:
        std::unique_ptr<window::Window> m_Window;
        bool m_Running = true;
    };

    Application *CreateApplication();
}