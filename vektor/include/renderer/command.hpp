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

        inline static void drawIndexed(const std::shared_ptr<utils::VertexArray> &vertexArray, uint32_t indexCount = 0)
        {
            s_RendererAPI->drawIndexed(vertexArray, indexCount);
        }

        inline static void init()
        {
            s_RendererAPI->init();
        };

        inline static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            s_RendererAPI->setViewport(x, y, width, height);
        }

    private:
        static renderer::RendererApi *s_RendererAPI;
    };
}