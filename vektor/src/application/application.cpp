#include "vk_pch.hpp"

#include "core/core.hpp"
#include "core/timestep.hpp"

#include "logger/logger.hpp"
#include "application/application.hpp"

#include "utils/shader.hpp"

#include "input/input.hpp"

#include "renderer/command.hpp"

#include "input/input.hpp"

namespace vektor
{
#define VEKTOR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

    Application *Application::s_Instance = nullptr;

    Application::Application()
    {
        VEKTOR_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        window::WindowProps props;
        props.title = "Vektor Engine";
        props.width = WINDOW_WIDTH;
        props.height = WINDOW_HEIGHT;

        m_Window = std::unique_ptr<window::Window>(window::Window::create(props));
        m_Window->setEventCallback(VEKTOR_BIND_EVENT_FN(Application::onEvent));

        m_ImGuiLayer = new imgui_layer::Layer();
        pushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        m_Shader->unbindProgram();

        m_VertexBuffer.reset();
        m_IndexBuffer.reset();

        m_VertexArray->unBind();

        m_Window.reset();
        s_Instance = nullptr;
        m_Shader.reset();
        VEKTOR_CORE_INFO("Application destroyed");
    }

    void Application::Run()
    {
        while (m_Running)
        {
            float time = (float)glfwGetTime();

            core::Timestep timestep = time - m_LastFrameTime;

            m_LastFrameTime = time;

            m_ImGuiLayer->begin();

            glDisable(GL_DEPTH_TEST);

            for (layer::Layer *layer : m_LayerStack)
            {
                layer->onRender();
            }

            m_ImGuiLayer->end();

            for (layer::Layer *layer : m_LayerStack)
            {
                layer->onUpdate(timestep);
            }

            m_Window->onUpdate();
        }
    }

    void Application::addEventListener(
        const std::function<void(event::Event &)> &listener)
    {
        m_EventListeners.emplace_back(listener);
    }

    void Application::pushLayer(layer::Layer *layer)
    {
        m_LayerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(layer::Layer *overlay)
    {
        m_LayerStack.pushOverlay(overlay);
        overlay->onAttach();
    }

    void Application::onEvent(event::Event &event)
    {
        event::EventDispatcher dispatcher(event);
        dispatcher.dispatch<event::WindowCloseEvent>(VEKTOR_BIND_EVENT_FN(Application::onWindowClose));
        dispatcher.dispatch<event::WindowResizeEvent>(VEKTOR_BIND_EVENT_FN(Application::onWindowResize));

        VEKTOR_CORE_TRACE("Event received: {}", event.toString());

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->onEvent(event);
            if (event.isHandled())
            {
                break;
            }
        }

        for (auto &listener : m_EventListeners)
        {
            listener(event);

            VEKTOR_CORE_INFO("Event dispatched to listener");

            if (event.isHandled())
            {
                break;
            }

            if (event.getEventType() == event::EventType::WindowResize)
            {
                event::WindowResizeEvent resizeEvent = static_cast<event::WindowResizeEvent &>(event);
                VEKTOR_CORE_TRACE("Window resized to {0}x{1}", resizeEvent.getWidth(), resizeEvent.getHeight());
            }
        }
    }

    bool Application::onWindowClose(event::WindowCloseEvent &event)
    {
        m_Running = false;
        return true;
    }

    bool Application::onWindowResize(event::WindowResizeEvent &event)
    {
        glViewport(0, 0, event.getWidth(), event.getHeight());
        return true;
    }
}