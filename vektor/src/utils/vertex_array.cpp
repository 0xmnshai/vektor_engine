
#include "opengl/vertex_array.hpp"
#include "renderer/api.hpp"
#include "utils/vertex_array.hpp"

namespace vektor::utils
{
    VertexArray *VertexArray::create()
    {
        switch (renderer::Renderer::getAPI())
        {
        case renderer::API::OPENGL:
            return new opengl::VertexArray();
        case renderer::API::NONE:
            VEKTOR_CORE_ASSERT(false, "Renderer API not set!");
            return nullptr;
        default:
            VEKTOR_CORE_ASSERT(false, "Renderer API not supported!");
            return nullptr;
        }
    }
}