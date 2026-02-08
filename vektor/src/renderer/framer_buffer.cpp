
#include "core/core.hpp"

#include "renderer/api.hpp"
#include "renderer/framer_buffer.hpp"
#include "opengl/framer_buffer.hpp"

namespace vektor::renderer
{
    std::shared_ptr<renderer::Framebuffer> renderer::Framebuffer::create(const FramebufferSpecification &spec)
    {
        switch (renderer::RendererApi::getAPI())
        {
        case renderer::RendererApi::API::OPENGL:
            return std::make_shared<opengl::Framebuffer>(spec);
        case renderer::RendererApi::API::NONE:
            VEKTOR_CORE_ASSERT(false, "Renderer API not set!");
            return nullptr;
        default:
            VEKTOR_CORE_ASSERT(false, "Renderer API not supported!");
            return nullptr;
        }
    };
}