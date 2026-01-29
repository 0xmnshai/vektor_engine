
#include "utils/buffer.hpp"
#include "renderer/api.hpp"
#include "logger/logger.hpp"
#include "opengl/buffer.hpp"

// In a real engine, you would check a Renderer::GetAPI() here
// For now, we assume OpenGL is the only backend.

namespace vektor::utils::buffer
{
    Vertex *Vertex::create(const std::vector<float> &vertices)
    {
        switch (renderer::Renderer::getAPI())
        {
        case renderer::API::OPENGL:
            return new opengl::VertexBuffer(vertices);
        case renderer::API::NONE:
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
        case renderer::API::OPENGL:
            return new opengl::IndexBuffer(indices);
        case renderer::API::NONE:
            VEKTOR_CORE_ASSERT(false, "Renderer API not set!");
            return nullptr;
        default:
            VEKTOR_CORE_ASSERT(false, "Renderer API not supported!");
            return nullptr;
        }
    }
}