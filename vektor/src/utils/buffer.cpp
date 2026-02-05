
#include "utils/buffer.hpp"
#include "renderer/api.hpp"
#include "logger/logger.hpp"
#include "opengl/buffer.hpp"
#include "renderer/api.hpp"

namespace vektor::utils::buffer
{
    Vertex *Vertex::create(const uint32_t size)
    {
        switch (renderer::Renderer::getAPI())
        {
        case renderer::RendererApi::API::OPENGL:
            return new opengl::VertexBuffer(size);
        case renderer::RendererApi::API::NONE:
            VEKTOR_CORE_ASSERT(false, "Renderer API not set!");
            return nullptr;
        default:
            VEKTOR_CORE_ASSERT(false, "Renderer API not supported!");
            return nullptr;
        }
    }

    Vertex *Vertex::create(const std::vector<float> &vertices)
    {
        switch (renderer::Renderer::getAPI())
        {
        case renderer::RendererApi::API::OPENGL:
            return new opengl::VertexBuffer(vertices);
        case renderer::RendererApi::API::NONE:
            VEKTOR_CORE_ASSERT(false, "Renderer API not set!");
            return nullptr;
        default:
            VEKTOR_CORE_ASSERT(false, "Renderer API not supported!");
            return nullptr;
        }
    }

    Index *Index::create(const std::vector<uint32_t> &indices)
    {
        switch (renderer::Renderer::getAPI())
        {
        case renderer::RendererApi::API::OPENGL:
            return new opengl::IndexBuffer(indices);
        case renderer::RendererApi::API::NONE:
            VEKTOR_CORE_ASSERT(false, "Renderer API not set!");
            return nullptr;
        default:
            VEKTOR_CORE_ASSERT(false, "Renderer API not supported!");
            return nullptr;
        }
    }
}