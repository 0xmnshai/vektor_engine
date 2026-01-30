#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "core/core.hpp"
#include "utils/vertex_array.hpp"

namespace vektor::renderer
{
    class VEKTOR_API RendererApi
    {
    public:
        enum class API
        {
            NONE = 0,
            OPENGL = 1
        };

        virtual void setClearColor(const glm::vec4 &color) = 0;
        virtual void clear() = 0;

        virtual void drawIndexed(const std::shared_ptr<utils::VertexArray> &vertexArray) = 0;

        inline static API getAPI() { return s_API; }

    private:
        static API s_API;
        static RendererApi *s_RendererAPI;
    };

    class VEKTOR_API Renderer
    {
    public:
        static void beginScene(); // const glm::mat4 &projection);
        static void endScene();

        static void submit(const std::shared_ptr<utils::VertexArray> &vertexArray);
        // const std::shared_ptr<Command> &command);

        inline static RendererApi::API getAPI() { return RendererApi::getAPI(); }
    };
}