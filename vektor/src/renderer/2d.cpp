
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/2d.hpp"
#include "renderer/command.hpp"

#include "utils/buffer.hpp"
#include "utils/shader.hpp"
#include "utils/vertex_array.hpp"

#include "opengl/shader.hpp"

namespace vektor::renderer
{
    struct Renderer2DData
    {
        std::shared_ptr<utils::VertexArray> quadVertexArray;
        std::shared_ptr<vektor::utils::buffer::Index> quadIndexBuffer;
        std::shared_ptr<vektor::utils::buffer::Vertex> quadVertexBuffer;
        std::shared_ptr<utils::Shader> flatColorShader;
    };

    static Renderer2DData *s_Data;

    void renderer::Renderer2D::init()
    {
        s_Data = new Renderer2DData();

        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f};

        s_Data->quadVertexArray.reset(utils::VertexArray::create());
        s_Data->quadVertexBuffer.reset(utils::buffer::Vertex::create(std::vector<float>(vertices, vertices + sizeof(vertices) / sizeof(float))));

        utils::buffer::Layout layout = {
            {utils::buffer::ShaderDataType::Float3, "a_Position"},
            {utils::buffer::ShaderDataType::Float4, "a_Color"},
            {utils::buffer::ShaderDataType::Float2, "a_TexCoord"}};

        s_Data->quadVertexBuffer->setLayout(layout);
        s_Data->quadVertexArray->addVertexBuffer(s_Data->quadVertexBuffer);

        std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};
        s_Data->quadIndexBuffer.reset(vektor::utils::buffer::Index::create(indices));
        s_Data->quadVertexArray->setIndexBuffer(s_Data->quadIndexBuffer);

        const std::string shaderPath = "/Users/lazycodebaker/Documents/Dev/CPP/vektor_engine/assets/shaders/flat_color.glsl";
        s_Data->flatColorShader.reset(utils::Shader::create(shaderPath));
    }

    void renderer::Renderer2D::shutdown()
    {
        delete s_Data;
    }

    void renderer::Renderer2D::beginScene(const std::shared_ptr<camera::Orthographic> &camera)
    {
        std::dynamic_pointer_cast<opengl::OpenGLShader>(s_Data->flatColorShader)->bindProgram();
        std::dynamic_pointer_cast<opengl::OpenGLShader>(s_Data->flatColorShader)->setUniformShaderMatrix("u_ViewProjection", camera->getViewProjectionMatrix());
        s_Data->quadVertexArray->bind();
    }

    void renderer::Renderer2D::endScene()
    {
    }

    void renderer::Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color)
    {
        auto openGLShader = std::dynamic_pointer_cast<vektor::opengl::OpenGLShader>(s_Data->flatColorShader);

        if (openGLShader)
        {
            openGLShader->bindProgram();

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
            openGLShader->setUniformShaderMatrix("u_Transform", transform);

            openGLShader->setUniform4f("u_Color", color);

            renderer::Command::drawIndexed(s_Data->quadVertexArray);
        }
    }

    void renderer::Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
    {
        drawQuad({position.x, position.y, 0.0f}, {size.x, size.y}, color);
    }
}