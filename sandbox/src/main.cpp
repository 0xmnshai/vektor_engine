#include "vektor.hpp"

class ExampleLayer : public vektor::layer::Layer
{

#define VEKTOR_BIND_EVENT_FN(fn) std::bind(&ExampleLayer::onKeyPressedEvent, this, std::placeholders::_1)
#define VEKTOR_BIND_RESIZE_FN(fn) std::bind(&ExampleLayer::onWindowResizeEvent, this, std::placeholders::_1)

public:
    ExampleLayer()
        : Layer("Example")
    {
        float aspectRatio = 1280.0f / 720.0f;
        float zoom = 0.9f;

        m_Camera = std::make_shared<vektor::renderer::camera::Orthographic>(
            -aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom);
        m_CameraPosition = {0.0f, 0.0f, 0.0f};

        // TRIANGLE
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f};

        m_VertexArray.reset(vektor::utils::VertexArray::create());
        m_VertexBuffer.reset(vektor::utils::buffer::Vertex::create(std::vector<float>(vertices, vertices + sizeof(vertices) / sizeof(float))));

        vektor::utils::buffer::Layout layout = {
            {vektor::utils::buffer::ShaderDataType::Float3, "a_Position"},
            {vektor::utils::buffer::ShaderDataType::Float4, "a_Color"}};

        m_VertexBuffer->setLayout(layout);
        m_VertexArray->addVertexBuffer(m_VertexBuffer);

        std::vector<uint32_t> indices = {0, 1, 2};
        m_IndexBuffer.reset(vektor::utils::buffer::Index::create(indices));
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

        m_Shader = std::make_unique<vektor::utils::Shader>(vertexSrc, fragmentSrc);

        VEKTOR_CORE_INFO("OpenGL rendering setup complete");
    }

    void onUpdate() override
    {
        if (vektor::input::Input::isKeyPressed(VEKTOR_KEY_R))
            m_CameraRotation += 1.0f;
        if (vektor::input::Input::isKeyPressed(VEKTOR_KEY_T))
            m_CameraRotation -= 1.0f;

        if (vektor::input::Input::isKeyPressed(VEKTOR_KEY_LEFT) || vektor::input::Input::isKeyPressed(VEKTOR_KEY_A))
            m_CameraPosition.x -= m_CameraMoveSpeed;
        if (vektor::input::Input::isKeyPressed(VEKTOR_KEY_RIGHT) || vektor::input::Input::isKeyPressed(VEKTOR_KEY_D))
            m_CameraPosition.x += m_CameraMoveSpeed;
        if (vektor::input::Input::isKeyPressed(VEKTOR_KEY_UP) || vektor::input::Input::isKeyPressed(VEKTOR_KEY_W))
            m_CameraPosition.y += m_CameraMoveSpeed;
        if (vektor::input::Input::isKeyPressed(VEKTOR_KEY_DOWN) || vektor::input::Input::isKeyPressed(VEKTOR_KEY_S))
            m_CameraPosition.y -= m_CameraMoveSpeed;

        m_Camera->setPosition(m_CameraPosition);
        m_Camera->setRotation(m_CameraRotation);

        vektor::renderer::Command::setClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        vektor::renderer::Command::clear();

        vektor::renderer::Renderer::beginScene(m_Camera);
        vektor::renderer::Renderer::submit(m_Shader, m_VertexArray);
        vektor::renderer::Renderer::endScene();
    }

    void onRender() override // imgui
    {
    }

    void onEvent(vektor::event::Event &event) override
    {
        vektor::event::EventDispatcher dispatcher(event);
        dispatcher.dispatch<vektor::event::KeyPressedEvent>(VEKTOR_BIND_EVENT_FN(ExampleLayer::onKeyPressedEvent));

        dispatcher.dispatch<vektor::event::WindowResizeEvent>(VEKTOR_BIND_RESIZE_FN(ExampleLayer::onWindowResizeEvent));
    }

    bool onKeyPressedEvent(vektor::event::KeyPressedEvent &event)
    {
        return false;
    }

    bool onWindowResizeEvent(vektor::event::WindowResizeEvent &event)
    {
        if (event.getWidth() == 0 || event.getHeight() == 0)
            return false;

        float aspectRatio = (float)event.getWidth() / (float)event.getHeight();
        float zoom = 0.9f;

        m_Camera = std::make_shared<vektor::renderer::camera::Orthographic>(
            -aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom);

        m_Camera->setPosition(m_CameraPosition);
        m_Camera->setRotation(m_CameraRotation);

        return false;
    }

private:
    std::shared_ptr<vektor::utils::Shader> m_Shader;
    std::shared_ptr<vektor::utils::VertexArray> m_VertexArray;

    std::shared_ptr<vektor::utils::buffer::Index> m_IndexBuffer;
    std::shared_ptr<vektor::utils::buffer::Vertex> m_VertexBuffer;

    std::shared_ptr<vektor::renderer::camera::Orthographic> m_Camera;

    glm::vec3 m_CameraPosition;
    float m_CameraRotation = 0.0f;

    float m_CameraMoveSpeed = 0.05f;
};

class Sandbox : public vektor::Application
{
public:
    Sandbox()
    {
        pushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {
    }
};

vektor::Application *vektor::createApplication()
{
    return new Sandbox();
}