
#include "sandbox2d.hpp"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D")
{
    float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
    m_CameraController = std::make_shared<vektor::renderer::camera::Controller>(aspectRatio);

    float vertices[] = {
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

    m_ShaderLibrary.load("/Users/lazycodebaker/Documents/Dev/CPP/vektor_engine/assets/shaders/flat_color.glsl");
}

void Sandbox2D::onAttach()
{
    VEKTOR_CORE_INFO("Sandbox2D layer attached");
}

void Sandbox2D::onDetach()
{
    VEKTOR_CORE_INFO("Sandbox2D layer detached");
}

void Sandbox2D::onUpdate(vektor::core::Timestep timestep)
{
    VEKTOR_CORE_TRACE("Delta time: {0} seconds", timestep.getSeconds());

    m_CameraController->onUpdate(timestep);

    vektor::renderer::Command::setClearColor({0.2f, 0.3f, 0.8f, 1.0f});
    vektor::renderer::Command::clear();

    vektor::renderer::Renderer::beginScene(m_CameraController->getCamera());

    auto flatColorShader = m_ShaderLibrary.get("flat_color");
    auto openGLShader = std::dynamic_pointer_cast<vektor::opengl::OpenGLShader>(flatColorShader);

    if (openGLShader)
    {
        openGLShader->bindProgram();
        openGLShader->setUniform4f("u_Color", m_Color);
    }

    glm::mat4 transform = glm::mat4(1.0f);
    vektor::renderer::Renderer::submit(flatColorShader, m_VertexArray, transform);

    vektor::renderer::Renderer::endScene();
}

void Sandbox2D::onRender()
{
    ImGui::Begin("Color Settings");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_Color));
    ImGui::End();
}

void Sandbox2D::onEvent(vektor::event::Event &event)
{
    m_CameraController->onEvent(event);
}