
#include "opengl/shader.hpp"
#include "opengl/vertex_array.hpp"

#include "renderer/api.hpp"
#include "utils/vertex_array.hpp"
#include "utils/shader.hpp"

namespace vektor::utils
{
    VertexArray *VertexArray::create()
    {
        switch (renderer::Renderer::getAPI())
        {
        case renderer::RendererApi::API::OPENGL:
            return new opengl::VertexArray();
        case renderer::RendererApi::API::NONE:
            VEKTOR_CORE_ASSERT(false, "Renderer API not set!");
            return nullptr;
        }
        VEKTOR_CORE_ASSERT(false, "Unknown Renderer API!");
        return nullptr;
    }

    utils::Shader *utils::Shader::create(const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        switch (renderer::Renderer::getAPI())
        {
        case renderer::RendererApi::API::OPENGL:
            return new opengl::OpenGLShader(vertexSrc, fragmentSrc);
        case renderer::RendererApi::API::NONE:
            VEKTOR_CORE_ASSERT(false, "Renderer API not set!");
            return nullptr;
        default:
            VEKTOR_CORE_ASSERT(false, "Renderer API not supported!");
            return nullptr;
        }
    }
}