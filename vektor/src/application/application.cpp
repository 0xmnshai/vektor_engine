#include "vk_pch.hpp"

#include "core/core.hpp"
#include "core/timestep.hpp"

#include "logger/logger.hpp"

#include "application/application.hpp"

#include "opengl/shader.hpp"

#include "input/input.hpp"
#include "input/input.hpp"

#include "renderer/api.hpp"

#include "utils/instrumentor.hpp"

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
        m_Window->setEventCallback([this](event::Event &e)
                                   { this->onEvent(e); });

        renderer::Renderer::init();

        m_ImGuiLayer = new imgui_layer::Layer();
        pushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        if (m_Shader)
        {
            auto glShader = std::dynamic_pointer_cast<opengl::OpenGLShader>(m_Shader);
            if (glShader)
                glShader->unbindProgram();
        }

        m_VertexBuffer.reset();
        m_IndexBuffer.reset();

        if (m_VertexArray)
            m_VertexArray->unBind();

        m_Window.reset();
        s_Instance = nullptr;

        VEKTOR_CORE_INFO("Application destroyed");
    }

    void Application::Run()
    {
        VEKTOR_PROFILE_FUNCTION();

        m_LastFrameTime = (float)glfwGetTime();

        while (m_Running)
        {
            VEKTOR_PROFILE_SCOPE("RunLoop");

            float time = (float)glfwGetTime();

            core::Timestep timestep = time - m_LastFrameTime;

            m_LastFrameTime = time;

            if (!m_windowMinimised)
            {
                for (layer::Layer *layer : m_LayerStack)
                {
                    layer->onUpdate(timestep);
                }
            }

            m_ImGuiLayer->begin();

            for (layer::Layer *layer : m_LayerStack)
            {
                layer->onRender();
            }

            m_ImGuiLayer->end();

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
    }

    void Application::pushOverlay(layer::Layer *overlay)
    {
        m_LayerStack.pushOverlay(overlay);
    }

    void Application::onEvent(event::Event &event)
    {
        event::EventDispatcher dispatcher(event);
        dispatcher.dispatch<event::WindowCloseEvent>([this](auto &&e)
                                                     { return onWindowClose(e); });
        dispatcher.dispatch<event::WindowResizeEvent>([this](auto &&e)
                                                      { return onWindowResize(e); });

        VEKTOR_CORE_TRACE("Event received: {}", event.toString());

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->onEvent(event);
            if (event.isHandled())
                break;
        };

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
        if (event.getWidth() == 0 || event.getHeight() == 0)
        {
            m_windowMinimised = true;
            return false;
        }

        m_windowMinimised = false;

        renderer::Renderer::onWindowResize(event.getWidth(), event.getHeight());

        return true;
    }
}