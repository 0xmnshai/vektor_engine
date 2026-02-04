#pragma once

#include "core/core.hpp"
#include "utils/texture.hpp"
#include "renderer/camera/orthographic.hpp"

namespace vektor::renderer
{
    class VEKTOR_API Renderer2D
    {
    public:
        static void init();
        static void shutdown();

        static void beginScene(const std::shared_ptr<camera::Orthographic> &camera);
        static void endScene();

        static void drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
        static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);

        static void drawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<utils::Texture> &texture);
        static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<utils::Texture> &texture);

    private:
        static std::shared_ptr<camera::Orthographic> s_SceneCamera;
    };

}