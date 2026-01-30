
#include "renderer/command.hpp"

namespace vektor::renderer
{
    RendererApi::API RendererApi::s_API = RendererApi::API::OPENGL;

    void Renderer::beginScene()
    {
    }

    void Renderer::endScene()
    {
    }

    void Renderer::submit(const std::shared_ptr<utils::VertexArray> &vertexArray)
    {
        renderer::Command::drawIndexed(vertexArray);
    }
}