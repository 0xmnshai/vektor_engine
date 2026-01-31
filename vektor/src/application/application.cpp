#include "vk_pch.hpp"

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
        m_Camera = std::make_shared<renderer::camera::Orthographic>(-2.0f, 2.0f, -0.9f, 0.9f);

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

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f};

        m_VertexArray.reset(utils::VertexArray::create());
        m_VertexBuffer.reset(utils::buffer::Vertex::create(std::vector<float>(vertices, vertices + sizeof(vertices) / sizeof(float))));

        utils::buffer::Layout layout = {
            {utils::buffer::ShaderDataType::Float3, "a_Position"},
            {utils::buffer::ShaderDataType::Float4, "a_Color"}};

        m_VertexBuffer->setLayout(layout);
        m_VertexArray->addVertexBuffer(m_VertexBuffer);

        std::vector<uint32_t> indices = {0, 1, 2};
        m_IndexBuffer.reset(utils::buffer::Index::create(indices));
        m_VertexArray->setIndexBuffer(m_IndexBuffer);

        std::string vertexSrc = R"(
            #version 410 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec3 v_Position;
            out vec4 v_Color;

            uniform mat4 u_ViewProjection;

            void main() {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 410 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main() {
                color = vec4(v_Position * 0.5 + 0.5, 1.0); 
                color = v_Color;
            }
        )";

        m_Shader = std::make_unique<utils::Shader>(vertexSrc, fragmentSrc);

        VEKTOR_CORE_INFO("OpenGL rendering setup complete");

        // SQUARE

        // m_SquareVertexArray.reset(utils::VertexArray::create());

        // float squareVertices[3 * 4] = {
        //     -0.75f, -0.75f, 0.0f, // 0: Bottom Left
        //     0.75f, -0.75f, 0.0f,  // 1: Bottom Right
        //     0.75f, 0.75f, 0.0f,   // 2: Top Right
        //     -0.75f, 0.75f, 0.0f   // 3: Top Left
        // };

        // m_VertexBufferSquare.reset(utils::buffer::Vertex::create(std::vector<float>(squareVertices, squareVertices + sizeof(squareVertices) / sizeof(float))));

        // utils::buffer::Layout layoutSquare = {
        //     {utils::buffer::ShaderDataType::Float3, "a_Position"},
        // };

        // m_VertexBufferSquare->setLayout(layoutSquare);
        // m_SquareVertexArray->addVertexBuffer(m_VertexBufferSquare);

        // std::vector<uint32_t> indicesSquare = {0, 1, 2, 2, 3, 0};
        // m_IndexBufferSquare.reset(utils::buffer::Index::create(indicesSquare));
        // m_SquareVertexArray->setIndexBuffer(m_IndexBufferSquare);

        // std::string vertexSrcSquare = R"(
        //     #version 410 core

        //     layout(location = 0) in vec3 a_Position;

        //     out vec3 v_Position;

        //     void main() {
        //         v_Position = a_Position;
        //         gl_Position = vec4(a_Position, 1.0);
        //     }
        // )";

        // std::string fragmentSrcSquare = R"(
        //     #version 410 core

        //     layout(location = 0) out vec4 color;

        //     in vec3 v_Position;

        //     void main() {
        //         color = vec4(0.2f, 0.3f, 0.8f, 1.0f);
        //     }
        // )";

        // m_ShaderSquare = std::make_unique<utils::Shader>(vertexSrcSquare, fragmentSrcSquare);
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
            m_ImGuiLayer->begin();

            renderer::Command::setClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
            renderer::Command::clear();

            renderer::Renderer::beginScene(m_Camera);

            glDisable(GL_DEPTH_TEST);

            m_Camera->setPosition(glm::vec3(0.5f, 0.5f, 0.0f));
            m_Camera->setRotation(45.0f);

            // m_Camera->setPosition({input::Input::getMousePosition().x, input::Input::getMousePosition().y, 0.0f});

            // m_ShaderSquare->bindProgram();

            // renderer::Renderer::submit(m_SquareVertexArray);
            renderer::Renderer::endScene();

            // m_Shader->bindProgram();
            // m_Shader->setUniformShaderMatrix("u_ViewProjection", m_Camera->getViewProjectionMatrix());

            renderer::Renderer::submit(m_Shader, m_VertexArray);

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