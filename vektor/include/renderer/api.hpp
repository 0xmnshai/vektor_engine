#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "utils/shader.hpp"
#include "opengl/shader.hpp"

#include "core/core.hpp"
#include "utils/vertex_array.hpp"

#include "renderer/camera/orthographic.hpp"

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

        virtual void init() = 0;

        virtual void drawIndexed(const std::shared_ptr<utils::VertexArray> &vertexArray) = 0;

        inline static API getAPI() { return s_API; }

    private:
        static API s_API;
        static RendererApi *s_RendererAPI;
    };

    class VEKTOR_API Renderer
    {
    public:
        static void beginScene(const std::shared_ptr<camera::Orthographic> &camera);
        static void endScene();

        static void init();

        static void submit(
            const std::shared_ptr<utils::Shader> &shader,
            const std::shared_ptr<utils::VertexArray> &vertexArray,
            const glm::mat4 &transform = glm::mat4(1.0f)); // later we can implement material / mesh here
        // const std::shared_ptr<Command> &command);

        inline static RendererApi::API getAPI() { return RendererApi::getAPI(); }

    private:
        static std::shared_ptr<camera::Orthographic> s_SceneCamera;

        struct SceneInfo
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneInfo *cs_SceneData;
    };
}