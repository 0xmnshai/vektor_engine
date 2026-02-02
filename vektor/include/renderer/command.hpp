#pragma once

#include "core/core.hpp"
#include "renderer/api.hpp"
#include "utils/vertex_array.hpp"

namespace vektor::renderer
{
    class VEKTOR_API Command
    {
    public:
        inline static void setClearColor(const glm::vec4 &color)
        {
            s_RendererAPI->setClearColor(color);
        }
        inline static void clear()
        {
            s_RendererAPI->clear();
        }

        inline static void drawIndexed(const std::shared_ptr<utils::VertexArray> &vertexArray)
        {
            s_RendererAPI->drawIndexed(vertexArray);
        }

        inline static void init()
        {
            s_RendererAPI->init();
        };

    private:
        static renderer::RendererApi *s_RendererAPI;
    };
}