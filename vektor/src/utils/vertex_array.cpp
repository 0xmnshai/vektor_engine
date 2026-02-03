
#include "opengl/shader.hpp"
#include "opengl/vertex_array.hpp"

#include "renderer/api.hpp"
#include "utils/vertex_array.hpp"

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
}