#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "vektor.hpp"
#include "opengl/texture.hpp"
#include "utils/shader.hpp"

class ExampleLayer : public vektor::layer::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
        float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
        float zoom = 0.9f;

        m_Position = {0.0f, 0.0f, 0.0f};
        m_Transform = glm::translate(glm::mat4(1.0f), m_Position);

        m_Camera = std::make_shared<vektor::renderer::camera::Orthographic>(
            -aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom);
        m_CameraPosition = {0.0f, 0.0f, 0.0f};

        // TRIANGLE
        // float vertices[] = {
        //     // position         // texcoord
        //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        //     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        //     0.0f, 0.5f, 0.0f, 0.5f, 1.0f};

        float vertices[] = {
            // Position           // Color             // UV
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f};

        m_VertexArray.reset(vektor::utils::VertexArray::create());
        m_VertexBuffer.reset(vektor::utils::buffer::Vertex::create(std::vector<float>(vertices, vertices + sizeof(vertices) / sizeof(float))));

        vektor::utils::buffer::Layout layout = {
            {vektor::utils::buffer::ShaderDataType::Float3, "a_Position"},
            {vektor::utils::buffer::ShaderDataType::Float4, "a_Color"},
            {vektor::utils::buffer::ShaderDataType::Float2, "a_TexCoord"}};

        m_VertexBuffer->setLayout(layout);
        m_VertexArray->addVertexBuffer(m_VertexBuffer);

        std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};
        m_IndexBuffer.reset(vektor::utils::buffer::Index::create(indices));
        m_VertexArray->setIndexBuffer(m_IndexBuffer);

        // Material material = new Material(m_Shader);
        // Material::Instance = new MaterialInstance(material);
        // maetrial->set("name",shader);
        // texture ..
        // using material in SceneRenderer::submit

        // std::string fragmentSrc = R"(
        //     #version 410 core

        //     layout(location = 0) out vec4 color;

        //     in vec2 v_TexCoord;

        //     void main() {
        //         color = vec4(v_TexCoord, 0.0, 1.0);
        //     }
        // )";

        // std::string vertexSrc = R"(
        //     #version 410 core

        //     layout(location = 0) in vec3 a_Position;
        //     layout(location = 1) in vec2 a_TexCoord;

        //     uniform mat4 u_ViewProjection;
        //     uniform mat4 u_Transform;

        //     out vec2 v_TexCoord;

        //     void main() {
        //         v_TexCoord = a_TexCoord;
        //         gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        //     }
        // )";

        std::string vertexSrc = R"(
            #version 410 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;
            layout(location = 2) in vec2 a_TexCoord;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec2 v_TexCoord;
            out vec4 v_Color;

            void main() {
                v_TexCoord = a_TexCoord;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 410 core

            layout(location = 0) out vec4 color;

            in vec2 v_TexCoord;
            in vec4 v_Color;

            uniform sampler2D u_Texture;
            uniform vec4 u_Color; 

            void main() { 
                color = texture(u_Texture, v_TexCoord) * v_Color * u_Color;
            }
        )";

        // m_Shader = std::make_shared<vektor::opengl::OpenGLShader>("texture",vertexSrc, fragmentSrc);
        auto textureShader = m_ShaderLibrary.load("/Users/lazycodebaker/Documents/Dev/CPP/vektor_engine/assets/shaders/texture.glsl");

        m_Texture = vektor::utils::Texture::create("/Users/lazycodebaker/Documents/Dev/CPP/vektor_engine/assets/texture.jpg");

        textureShader->bindProgram();
        std::dynamic_pointer_cast<vektor::opengl::OpenGLShader>(textureShader)->setUniform1i("u_Texture", 0);
    }

    void onAttach() override
    {
        VEKTOR_CORE_INFO("OpenGL rendering setup complete");
    }

    void onUpdate(vektor::core::Timestep timestep) override
    {
        VEKTOR_CORE_TRACE("Delta time: {0} seconds", timestep.getSeconds());

        if (vektor::input::Input::isKeyPressed(VEKTOR_KEY_R))
            m_CameraRotation += m_CameraRotateSpeed * timestep;
        if (vektor::input::Input::isKeyPressed(VEKTOR_KEY_T))
            m_CameraRotation -= m_CameraRotateSpeed * timestep;

        if (vektor::input::Input::isKeyPressed(VEKTOR_KEY_LEFT) || vektor::input::Input::isKeyPressed(VEKTOR_KEY_A))
            m_CameraPosition.x -= m_CameraMoveSpeed * timestep;
        if (vektor::input::Input::isKeyPressed(VEKTOR_KEY_RIGHT) || vektor::input::Input::isKeyPressed(VEKTOR_KEY_D))
            m_CameraPosition.x += m_CameraMoveSpeed * timestep;
        if (vektor::input::Input::isKeyPressed(VEKTOR_KEY_UP) || vektor::input::Input::isKeyPressed(VEKTOR_KEY_W))
            m_CameraPosition.y += m_CameraMoveSpeed * timestep;
        if (vektor::input::Input::isKeyPressed(VEKTOR_KEY_DOWN) || vektor::input::Input::isKeyPressed(VEKTOR_KEY_S))
            m_CameraPosition.y -= m_CameraMoveSpeed * timestep;

        m_Camera->setPosition(m_CameraPosition);
        m_Camera->setRotation(m_CameraRotation);

        vektor::renderer::Command::setClearColor(m_Color);
        vektor::renderer::Command::clear();

        vektor::renderer::Renderer::beginScene(m_Camera);

        // TEST
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        glm::vec4 redColor(1.0f, 0.0f, 0.0f, 1.0f);
        glm::vec4 greenColor(0.0f, 1.0f, 0.0f, 1.0f);
        glm::vec4 blueColor(0.0f, 0.0f, 1.0f, 1.0f);

        m_Texture->bind(0);

        auto textureShader = m_ShaderLibrary.get("texture");

        for (size_t y = 0; y < 5; y++)
        {
            for (size_t i = 0; i < 5; i++)
            {
                glm::vec3 pos(i * 0.2f, y * 0.2f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                m_Transform = transform;

                if ((i + y) % 2 == 0) // RED
                {
                    std::dynamic_pointer_cast<vektor::opengl::OpenGLShader>(textureShader)->setUniformMat4("u_Color", redColor);
                    vektor::renderer::Renderer::submit(textureShader, m_VertexArray, transform);
                }
                else // BLACK ( blue + red )
                {
                    std::dynamic_pointer_cast<vektor::opengl::OpenGLShader>(textureShader)->setUniformMat4("u_Color", blueColor);
                    vektor::renderer::Renderer::submit(textureShader, m_VertexArray, transform);
                }
            }
        }
        // TEST
        vektor::renderer::Renderer::endScene();
    }

    void onRender() override // imgui
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Clear Color", glm::value_ptr(m_Color));
        ImGui::End();
    }

    void onEvent(vektor::event::Event &event) override
    {
        vektor::event::EventDispatcher dispatcher(event);
        dispatcher.dispatch<vektor::event::WindowResizeEvent>(VEKTOR_BIND_EVENT_FN(ExampleLayer::onWindowResizeEvent));
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
    // std::shared_ptr<vektor::utils::Shader> m_Shader, m_TextureShader;
    std::shared_ptr<vektor::utils::VertexArray> m_VertexArray;

    vektor::utils::ShaderLibrary m_ShaderLibrary;

    std::shared_ptr<vektor::utils::buffer::Index> m_IndexBuffer;
    std::shared_ptr<vektor::utils::buffer::Vertex> m_VertexBuffer;

    std::shared_ptr<vektor::utils::Texture> m_Texture;

    std::shared_ptr<vektor::renderer::camera::Orthographic> m_Camera;

    glm::vec3 m_CameraPosition;
    float m_CameraRotation = 0.0f;

    float m_CameraRotateSpeed = 50.0f;
    float m_CameraMoveSpeed = 5.0f;

    glm::vec3 m_Position;
    glm::mat4 m_Transform;

    glm::vec4 m_Color = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
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