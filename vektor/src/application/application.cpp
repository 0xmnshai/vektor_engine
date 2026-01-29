#include "vk_pch.hpp"

#include "logger/logger.hpp"
#include "application/application.hpp"

#include "utils/shader.hpp"

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
        props.width = 1280;
        props.height = 720;

        m_Window = std::unique_ptr<window::Window>(window::Window::create(props));
        m_Window->setEventCallback(VEKTOR_BIND_EVENT_FN(Application::onEvent));

        m_ImGuiLayer = new imgui_layer::Layer();
        pushOverlay(m_ImGuiLayer);

        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f};

        m_VertexBuffer.reset(utils::buffer::Vertex::create(std::vector<float>(vertices, vertices + sizeof(vertices) / sizeof(float))));
        m_VertexBuffer->bind();

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

        std::vector<uint32_t> indices = {0, 1, 2};
        m_IndexBuffer.reset(utils::buffer::Index::create(indices));
        m_IndexBuffer->bind(); // Binds to GL_ELEMENT_ARRAY_BUFFER

        glBindVertexArray(0);

        std::string vertexSrc = R"(
            #version 410 core
            layout(location = 0) in vec3 a_Position;
            void main() {
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 410 core
            layout(location = 0) out vec4 color;
            void main() {
                color = vec4(0.8, 0.2, 0.3, 1.0); // Reddish color
            }
        )";

        m_Shader = std::make_unique<utils::Shader>(vertexSrc, fragmentSrc);

        VEKTOR_CORE_INFO("OpenGL rendering setup complete");
    }

    Application::~Application()
    {
        m_Shader->unbindProgram();

        m_VertexBuffer.reset();
        m_IndexBuffer.reset();

        glDeleteVertexArrays(1, &m_VertexArray);

        m_Window.reset();
        s_Instance = nullptr;
        m_Shader.reset();
        VEKTOR_CORE_INFO("Application destroyed");
    }

    void Application::Run()
    {
        while (m_Running)
        {
            m_ImGuiLayer->begin();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_Shader->bindProgram();

            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);

            for (layer::Layer *layer : m_LayerStack)
            {
                layer->onRender();
            }

            m_ImGuiLayer->end();

            for (layer::Layer *layer : m_LayerStack)
            {
                layer->onUpdate();
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