
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/2d.hpp"
#include "renderer/command.hpp"

#include "utils/buffer.hpp"
#include "utils/shader.hpp"
#include "utils/vertex_array.hpp"

#include "opengl/shader.hpp"
#include "opengl/texture.hpp"

namespace vektor::renderer
{

    struct QuadVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;
        float texIndex;
        float tilingFactor;
    };

    struct Renderer2DData
    {
        const int MAX_QUADS = 10000;
        const int MAX_VERTICES = MAX_QUADS * 4;
        const int MAX_INDICES = MAX_QUADS * 6;
        const int MAX_TEXTURE_SLOTS = 32;

        std::shared_ptr<utils::VertexArray> quadVertexArray;
        std::shared_ptr<vektor::utils::buffer::Index> quadIndexBuffer;
        std::shared_ptr<vektor::utils::buffer::Vertex> quadVertexBuffer;

        std::shared_ptr<utils::Shader> flatTextureShader;
        std::shared_ptr<opengl::OpenGLTexture2D> whiteTexture;

        QuadVertex *quadVertexBufferBase = nullptr;
        QuadVertex *quadVertexBufferPtr = nullptr;

        uint32_t quadIndexCount = 0;
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

        s_Data->quadVertexBufferBase = new QuadVertex[s_Data->MAX_VERTICES];

        s_Data->quadVertexArray.reset(utils::VertexArray::create());
        // s_Data->quadVertexBuffer.reset(utils::buffer::Vertex::create(std::vector<float>(vertices, vertices + sizeof(vertices) / sizeof(float))));
        s_Data->quadVertexBuffer.reset(utils::buffer::Vertex::create(s_Data->MAX_VERTICES * sizeof(QuadVertex)));

        utils::buffer::Layout layout = {
            {utils::buffer::ShaderDataType::Float3, "a_Position"},
            {utils::buffer::ShaderDataType::Float4, "a_Color"},
            {utils::buffer::ShaderDataType::Float2, "a_TexCoord"}};

        s_Data->quadVertexBuffer->setLayout(layout);
        s_Data->quadVertexArray->addVertexBuffer(s_Data->quadVertexBuffer);

        std::vector<uint32_t> quadIndices;
        quadIndices.resize(s_Data->MAX_INDICES);

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data->MAX_INDICES; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        // s_Data->quadIndexBuffer.reset(vektor::utils::buffer::Index::create(quadIndices));
        // std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};
        s_Data->quadIndexBuffer.reset(vektor::utils::buffer::Index::create(quadIndices));
        s_Data->quadVertexArray->setIndexBuffer(s_Data->quadIndexBuffer);

        s_Data->whiteTexture = std::make_shared<opengl::OpenGLTexture2D>(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data->whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

        const std::string shaderPath = "/Users/lazycodebaker/Documents/Dev/CPP/vektor_engine/assets/shaders/flat_color.glsl";
        const std::string textureShaderPath = "/Users/lazycodebaker/Documents/Dev/CPP/vektor_engine/assets/shaders/texture.glsl";

        s_Data->flatTextureShader.reset(utils::Shader::create(textureShaderPath));
    }

    void renderer::Renderer2D::shutdown()
    {
        delete s_Data;
    }

    void renderer::Renderer2D::beginScene(const std::shared_ptr<camera::Orthographic> &camera)
    {
        std::dynamic_pointer_cast<opengl::OpenGLShader>(s_Data->flatTextureShader)->bindProgram();
        std::dynamic_pointer_cast<opengl::OpenGLShader>(s_Data->flatTextureShader)->setUniformShaderMatrix("u_ViewProjection", camera->getViewProjectionMatrix());

        s_Data->quadVertexArray->bind();
        s_Data->quadVertexBufferPtr = s_Data->quadVertexBufferBase;
        s_Data->quadIndexCount = 0;
    }

    void flush()
    {
    } 

    void renderer::Renderer2D::endScene()
    {
        uint32_t dataSize = (uint32_t)((uint8_t *)s_Data->quadVertexBufferPtr - (uint8_t *)s_Data->quadVertexBufferBase);
        s_Data->quadVertexBuffer->setData(s_Data->quadVertexBufferBase, dataSize);

        s_Data->quadIndexBuffer->bind();
        renderer::Command::drawIndexed(s_Data->quadVertexArray, s_Data->quadIndexCount);

        flush();
    }

    void renderer::Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color)
    {
        std::shared_ptr<vektor::opengl::OpenGLShader> openGLShader = std::dynamic_pointer_cast<vektor::opengl::OpenGLShader>(s_Data->flatTextureShader);

        if (openGLShader)
        {

            s_Data->quadVertexBufferPtr->position = position;
            s_Data->quadVertexBufferPtr->color = color;
            s_Data->quadVertexBufferPtr->texCoord = {0.0f, 0.0f};
            s_Data->quadVertexBufferPtr->texIndex = 0.0f;
            s_Data->quadVertexBufferPtr->tilingFactor = 1.0f;
            s_Data->quadVertexBufferPtr++;

            s_Data->quadVertexBufferPtr->position = {position.x + size.x, position.y, position.z};
            s_Data->quadVertexBufferPtr->color = color;
            s_Data->quadVertexBufferPtr->texCoord = {1.0f, 0.0f};
            s_Data->quadVertexBufferPtr->texIndex = 0.0f;
            s_Data->quadVertexBufferPtr->tilingFactor = 1.0f;
            s_Data->quadVertexBufferPtr++;

            s_Data->quadVertexBufferPtr->position = {position.x + size.x, position.y + size.y, position.z};
            s_Data->quadVertexBufferPtr->color = color;
            s_Data->quadVertexBufferPtr->texCoord = {1.0f, 1.0f};
            s_Data->quadVertexBufferPtr->texIndex = 0.0f;
            s_Data->quadVertexBufferPtr->tilingFactor = 1.0f;
            s_Data->quadVertexBufferPtr++;

            s_Data->quadVertexBufferPtr->position = {position.x + size.x, position.y + size.y, position.z};
            s_Data->quadVertexBufferPtr->color = color;
            s_Data->quadVertexBufferPtr->texCoord = {0.0f, 1.0f};
            s_Data->quadVertexBufferPtr->texIndex = 0.0f;
            s_Data->quadVertexBufferPtr->tilingFactor = 1.0f;
            s_Data->quadVertexBufferPtr++;

            s_Data->quadIndexCount += 6;

            openGLShader->bindProgram();

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
            openGLShader->setUniformShaderMatrix("u_Transform", transform);

            openGLShader->setUniform4f("u_Color", color);
            s_Data->whiteTexture->bind();
            // openGLShader->setUniform1i("u_Texture", 0);

            renderer::Command::drawIndexed(s_Data->quadVertexArray);
        }
    }

    void renderer::Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
    {
        drawQuad({position.x, position.y, 0.0f}, {size.x, size.y}, color);
    }

    void renderer::Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<utils::Texture> &texture, glm::vec4 tintColor)
    {
        drawQuad({position.x, position.y, 0.0f}, {size.x, size.y}, texture, tintColor);
    }

    void renderer::Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<utils::Texture> &texture, glm::vec4 tintColor)
    {
        std::shared_ptr<vektor::opengl::OpenGLShader> openGLShader = std::dynamic_pointer_cast<vektor::opengl::OpenGLShader>(s_Data->flatTextureShader);

        if (openGLShader)
        {
            openGLShader->bindProgram();

            openGLShader->setUniform4f("u_Color", tintColor);

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
            openGLShader->setUniformShaderMatrix("u_Transform", transform);

            texture->bind();
            openGLShader->setUniform1i("u_Texture", 0);

            renderer::Command::drawIndexed(s_Data->quadVertexArray);
        }

        openGLShader->unbindProgram();
    }

    void renderer::Renderer2D::drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color)
    {
        drawRotatedQuad({position.x, position.y, 0.0f}, {size.x, size.y}, rotation, color);
    }

    void renderer::Renderer2D::drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color)
    {
        std::shared_ptr<vektor::opengl::OpenGLShader> openGLShader = std::dynamic_pointer_cast<vektor::opengl::OpenGLShader>(s_Data->flatTextureShader);

        if (openGLShader)
        {
            openGLShader->bindProgram();

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
            openGLShader->setUniformShaderMatrix("u_Transform", transform);

            openGLShader->setUniform4f("u_Color", color);
            s_Data->whiteTexture->bind();
            // openGLShader->setUniform1i("u_Texture", 0);

            renderer::Command::drawIndexed(s_Data->quadVertexArray);
        }
    }

    void renderer::Renderer2D::drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const Ref<utils::Texture> &texture, glm::vec4 tintColor)
    {
        drawRotatedQuad({position.x, position.y, 0.0f}, {size.x, size.y}, rotation, texture, tintColor);
    }

    void renderer::Renderer2D::drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const Ref<utils::Texture> &texture, glm::vec4 tintColor)
    {
        std::shared_ptr<vektor::opengl::OpenGLShader> openGLShader = std::dynamic_pointer_cast<vektor::opengl::OpenGLShader>(s_Data->flatTextureShader);

        if (openGLShader)
        {
            openGLShader->bindProgram();

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
            openGLShader->setUniformShaderMatrix("u_Transform", transform);

            openGLShader->setUniform4f("u_Color", tintColor);

            texture->bind();
            openGLShader->setUniform1i("u_Texture", 0);

            renderer::Command::drawIndexed(s_Data->quadVertexArray);
        }
    }
}