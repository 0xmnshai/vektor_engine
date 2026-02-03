#pragma once

#include <glm/glm.hpp>

#include "core/core.hpp"

namespace vektor::renderer::camera
{
    class VEKTOR_API Orthographic
    {
    public:
        Orthographic(float left, float right, float bottom, float top); // left and right are the x-axis limits, bottom and top are the y-axis limits.
        ~Orthographic();

        void setProjection(float left, float right, float bottom, float top);

        inline const glm::vec3 &getPosition() const { return m_Position; }
        inline void setPosition(const glm::vec3 &position)
        {
            m_Position = position;
            reCalculateViewMatrix();
        }

        inline const glm::mat4 &getProjectionMatrix() const { return m_projectionMatrix; }
        inline const glm::mat4 &getViewMatrix() const { return m_viewMatrix; }
        inline const glm::mat4 &getViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

        inline float getRotation() const { return m_Rotation; }
        inline void setRotation(float rotation)
        {
            m_Rotation = rotation;
            reCalculateViewMatrix();
        }

    private:
        glm::mat4 m_projectionMatrix;
        glm::mat4 m_viewMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position;

        float m_Rotation = 0.0f;

        void reCalculateViewMatrix();
    };
}