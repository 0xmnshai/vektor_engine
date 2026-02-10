
#include "world/scene/camera.hpp"

namespace vektor::world::scene
{
    SceneCamera::SceneCamera()
    {
        recalculateProjectionMatrix();
    }
  
    void SceneCamera::setProjection(float fov, float aspectRatio, float nearClip, float farClip)
    {
        m_AspectRatio = aspectRatio;
        m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

        recalculateProjectionMatrix();
    }

    void SceneCamera::setOrthographic(float size, float nearClip, float farClip)
    {
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;

        recalculateProjectionMatrix();
    }

    void SceneCamera::recalculateProjectionMatrix()
    {
        m_ProjectionMatrix = glm::ortho(-m_OrthographicSize * m_AspectRatio, m_OrthographicSize * m_AspectRatio, -m_OrthographicSize, m_OrthographicSize, m_OrthographicNear, m_OrthographicFar);
    }

    void SceneCamera::setViewPortSize(uint32_t width, uint32_t height)
    {
        m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
        recalculateProjectionMatrix();
    }
}