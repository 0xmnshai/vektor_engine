#pragma once

#include <vector>

#include "core/core.hpp"

#include "window/window.hpp"

#include "events/window.hpp"
#include "events/mouse.hpp"
#include "events/keyboard.hpp"

namespace vektor
{
    class VEKTOR_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
        void onEvent(event::Event &event);
        
        void addEventListener(const std::function<void(event::Event &)> &listener);

    private:
        std::unique_ptr<window::Window> m_Window;
        bool m_Running = true;

        std::vector<std::function<void(event::Event &)>> m_EventListeners;

        bool onWindowClose(event::WindowCloseEvent &e);
        bool onWindowResize(event::WindowResizeEvent &e);
    };

    Application *CreateApplication();
}