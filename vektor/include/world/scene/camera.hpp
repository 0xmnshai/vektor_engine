#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/core.hpp"

namespace vektor::world::scene
{
    class VEKTOR_API SceneCamera
    {
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        const glm::mat4 &getProjectionMatrix() const { return m_ProjectionMatrix; }

        void setProjection(float fov, float aspectRatio, float nearClip, float farClip);
        void setOrthographic(float size, float nearClip, float farClip);

        void setViewPortSize(uint32_t width, uint32_t height);

        void recalculateProjectionMatrix();

    private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;

        float m_AspectRatio = 1.0f;

        float m_OrthographicSize = 10.0f; // half-height
        float m_OrthographicNear = -1.0f;
        float m_OrthographicFar = 1.0f;
    };
}