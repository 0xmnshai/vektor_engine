
#include "core/core.hpp"
#include "renderer/api.hpp"

namespace vektor::renderer
{
    class VEKTOR_API OpenGLAPI : public renderer::RendererApi
    {
        virtual void init() override;

        virtual void setClearColor(const glm::vec4 &color) override;
        virtual void clear() override;

        virtual void drawIndexed(const std::shared_ptr<utils::VertexArray> &vertexArray) override;
    };
}