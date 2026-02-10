#pragma once

#include "core/core.hpp"
#include "utils/texture.hpp"
#include "utils/subtexture.hpp"
#include "renderer/camera/orthographic.hpp"
#include "world/ecs/component_storage/component.hpp"

namespace vektor::renderer
{
    class VEKTOR_API Renderer2D
    {
    public:
        struct Statistics
        {
            uint32_t drawCalls = 0;
            uint32_t quadCount = 0;
            uint32_t getQuadCount() { return quadCount; }
            uint32_t getDrawCalls() { return drawCalls; }
            uint32_t getTotalVertexCount() { return quadCount * 4; }
            uint32_t getTotalIndexCount() { return quadCount * 6; }
        };

        static void init();
        static void shutdown();

        static void beginScene(const world::ecs::component_storage::CameraComponent &cameraComponent,
                               const world::ecs::component_storage::TransformComponent &transformComponent);

        static void beginScene(const std::shared_ptr<camera::Orthographic> &camera);

        static void endScene();

        static void drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
        static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
        static void drawQuad(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<utils::Texture> &texture, glm::vec4 tintColor);
        static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<utils::Texture> &texture, glm::vec4 tintColor);
        static void drawQuad(const glm::vec2 &position, const glm::vec2 &size, const std::shared_ptr<utils::SubTexture2D> &texture, glm::vec4 tintColor);
        static void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const std::shared_ptr<utils::SubTexture2D> &texture, glm::vec4 tintColor);

        static void drawQuad(const glm::mat4 &transform, glm::vec4 tintColor);
        static void drawQuad(const glm::mat4 &transform, const std::shared_ptr<utils::SubTexture2D> &texture, glm::vec4 tintColor);

        static void drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color);
        static void drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color);
        static void drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const std::shared_ptr<utils::Texture> &texture, glm::vec4 tintColor);
        static void drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const std::shared_ptr<utils::Texture> &texture, glm::vec4 tintColor);
        static void drawRotatedQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const std::shared_ptr<utils::SubTexture2D> &texture, glm::vec4 tintColor);
        static void drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const std::shared_ptr<utils::SubTexture2D> &texture, glm::vec4 tintColor);

        static void resetStats();
        static Statistics getStats();

    private:
        static std::shared_ptr<camera::Orthographic> s_SceneCamera;

        static void startNewBatch();
    };
}