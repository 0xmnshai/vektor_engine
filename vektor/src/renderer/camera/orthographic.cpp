
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/camera/orthographic.hpp"

namespace vektor::renderer::camera
{
    Orthographic::Orthographic(float left, float right, float bottom, float top)
        : m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
          m_viewMatrix(glm::mat4(1.0f)),
          m_ViewProjectionMatrix(glm::mat4(1.0f))
    {
        m_ViewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

    void Orthographic:: setProjection(float left, float right, float bottom, float top)
    {
        m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

    void Orthographic::reCalculateViewMatrix()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
        transform = glm::rotate(transform, glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        m_viewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

    Orthographic::~Orthographic()
    {
        m_projectionMatrix = glm::mat4(1.0f);
        m_viewMatrix = glm::mat4(1.0f);
        m_ViewProjectionMatrix = glm::mat4(1.0f);
    }
}