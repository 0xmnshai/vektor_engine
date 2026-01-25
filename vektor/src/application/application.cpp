
#include "vk_pch.hpp"

#include "application/application.hpp"

namespace vektor
{
    Application::Application()
    {
        window::WindowProps props;
        props.title = "Vektor Engine";
        props.width = 1280;
        props.height = 720;

        m_Window = std::unique_ptr<window::Window>(window::Window::create(props));
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_Running)
        {
            m_Window->onUpdate();
        }
    }
}
