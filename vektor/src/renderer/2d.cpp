#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/2d.hpp"
#include "renderer/command.hpp"
#include "utils/buffer.hpp"
#include "utils/vertex_array.hpp"
#include "utils/shader.hpp"
#include "opengl/shader.hpp"
#include "opengl/texture.hpp"

namespace vektor::renderer
{
    struct QuadVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;
        // float texIndex;
        // float tilingFactor;
    };

    struct Renderer2DData
    {
        static const uint32_t MAX_QUADS = 20000;
        static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
        static const uint32_t MAX_INDICES = MAX_QUADS * 6;

        std::shared_ptr<utils::VertexArray> quadVertexArray;
        std::shared_ptr<vektor::utils::buffer::Vertex> quadVertexBuffer;
        std::shared_ptr<utils::Shader> textureShader;

        std::shared_ptr<utils::Texture> whiteTexture;
        std::shared_ptr<utils::Texture> currentTexture;

        uint32_t quadIndexCount = 0;
        QuadVertex *quadVertexBufferBase = nullptr;
        QuadVertex *quadVertexBufferPtr = nullptr;

        glm::vec4 quadVertexPositions[4];
    };

    static Renderer2DData s_Data;

    void flushAndReset()
    {
        if (s_Data.quadIndexCount == 0)
            return;

        uint32_t dataSize = (uint32_t)((uint8_t *)s_Data.quadVertexBufferPtr - (uint8_t *)s_Data.quadVertexBufferBase);
        s_Data.quadVertexBuffer->setData(s_Data.quadVertexBufferBase, dataSize);

        if (s_Data.currentTexture)
            s_Data.currentTexture->bind(0);

        s_Data.quadVertexArray->bind();
        renderer::Command::drawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);

        s_Data.quadIndexCount = 0;
        s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;
    }

    void Renderer2D::init()
    {
        s_Data.quadVertexBufferBase = new QuadVertex[s_Data.MAX_VERTICES];

        s_Data.quadVertexArray.reset(utils::VertexArray::create());
        s_Data.quadVertexBuffer.reset(utils::buffer::Vertex::create(s_Data.MAX_VERTICES * sizeof(QuadVertex)));

        utils::buffer::Layout layout = {
            {utils::buffer::ShaderDataType::Float3, "a_Position"},
            {utils::buffer::ShaderDataType::Float4, "a_Color"},
            {utils::buffer::ShaderDataType::Float2, "a_TexCoord"}};

        s_Data.quadVertexBuffer->setLayout(layout);
        s_Data.quadVertexArray->addVertexBuffer(s_Data.quadVertexBuffer);

        uint32_t *quadIndices = new uint32_t[s_Data.MAX_INDICES];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MAX_INDICES; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        std::shared_ptr<vektor::utils::buffer::Index> quadIB;
        quadIB.reset(vektor::utils::buffer::Index::create(std::vector<uint32_t>(quadIndices, quadIndices + s_Data.MAX_INDICES)));
        s_Data.quadVertexArray->setIndexBuffer(quadIB);
        delete[] quadIndices;

        s_Data.whiteTexture = utils::Texture2D::create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        std::dynamic_pointer_cast<opengl::OpenGLTexture2D>(s_Data.whiteTexture)->setData(&whiteTextureData, sizeof(uint32_t));

        s_Data.textureShader.reset(utils::Shader::create("/Users/lazycodebaker/Documents/Dev/CPP/vektor_engine/assets/shaders/texture.glsl"));
        s_Data.textureShader->bindProgram();
        std::dynamic_pointer_cast<opengl::OpenGLShader>(s_Data.textureShader)->setUniform1i("u_Texture", 0);

        s_Data.quadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
        s_Data.quadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
        s_Data.quadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
        s_Data.quadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
    }

    void Renderer2D::shutdown()
    {
        delete[] s_Data.quadVertexBufferBase;
    }

    void Renderer2D::beginScene(const std::shared_ptr<camera::Orthographic> &camera)
    {
        s_Data.textureShader->bindProgram();
        std::dynamic_pointer_cast<opengl::OpenGLShader>(s_Data.textureShader)->setUniformShaderMatrix("u_ViewProjection", camera->getViewProjectionMatrix());

        s_Data.quadIndexCount = 0;
        s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;
        s_Data.currentTexture = s_Data.whiteTexture;
    }

    void Renderer2D::endScene()
    {
        flushAndReset();
    }

    void Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color)
    {
        drawQuad({position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color)
    {
        drawRotatedQuad(position, size, 0.0f, s_Data.whiteTexture, color);
    }

    void Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<utils::Texture> &texture, glm::vec4 tintColor)
    {
        drawQuad({position.x, position.y, 0.0f}, size, texture, tintColor);
    }

    void Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<utils::Texture> &texture, glm::vec4 tintColor)
    {
        drawRotatedQuad(position, size, 0.0f, texture, tintColor);
    }

    void Renderer2D::drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color)
    {
        drawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, s_Data.whiteTexture, color);
    }

    void Renderer2D::drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const std::shared_ptr<utils::Texture> &texture, glm::vec4 tintColor)
    {
        drawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tintColor);
    }

    void Renderer2D::drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const std::shared_ptr<utils::Texture> &texture, glm::vec4 tintColor)
    {
        if (s_Data.quadIndexCount >= Renderer2DData::MAX_INDICES)
        {
            flushAndReset();
        }

        if (s_Data.currentTexture != texture)
        {
            flushAndReset();
            s_Data.currentTexture = texture;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

        for (int i = 0; i < 4; i++)
        {
            s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[i];
            s_Data.quadVertexBufferPtr->color = tintColor;
            s_Data.quadVertexBufferPtr->texCoord = textureCoords[i];
            s_Data.quadVertexBufferPtr++;
        }

        s_Data.quadIndexCount += 6;
    }
}