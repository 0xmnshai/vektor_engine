#pragma once

#include <glm/glm.hpp>

#include "core/core.hpp"
#include "core/timestep.hpp"

#include "renderer/camera/orthographic.hpp"

#include "events/event.hpp"
#include "events/mouse.hpp"
#include "events/window.hpp"

namespace vektor::renderer::camera
{
    class VEKTOR_API Controller
    {
    public:
        Controller(float aspectRatio, float zoomLevel = 1.0f, bool rotation = false);

        void onUpdate(const core::Timestep timestep);
        void onEvent(event::Event &event);

        inline const std::shared_ptr<camera::Orthographic> &getCamera() const { return m_Camera; }
        inline void setCamera(const std::shared_ptr<camera::Orthographic> &camera) { m_Camera = camera; }

        inline const glm::vec3 &getCameraPosition() const { return m_CameraPosition; }
        inline void setCameraPosition(const glm::vec3 &position) { m_CameraPosition = position; }

        inline float getCameraRotation() const { return m_CameraRotation; }
        inline void setCameraRotation(float rotation) { m_CameraRotation = rotation; }

        inline bool getRotation() const { return m_Rotation; }
        inline void setRotation(bool rotation) { m_Rotation = rotation; }

        inline float getCameraTranslationSpeed() const { return m_CameraTranslationSpeed; }
        inline void setCameraTranslationSpeed(float speed) { m_CameraTranslationSpeed = speed; }

        inline float getZoomLevel() const { return m_ZoomLevel; }
        inline void setZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }

    private:
        float m_AspectRatio;
        bool m_Rotation;

        float m_ZoomLevel = 1.0f;
        float m_TargetZoomLevel = 1.0f;
        float m_ZoomSmoothSpeed = 8.0f;

        glm::vec3 m_CameraPosition;
        float m_CameraRotation = 0.0f;

        float m_CameraTranslationSpeed = 10.0f;
        float m_CameraRotationSpeed = 180.0f;

        bool onMouseScrolled(const event::MouseScrolledEvent &event);
        bool onWindowResize(const event::WindowResizeEvent &event);

        std::shared_ptr<camera::Orthographic> m_Camera;
    };
}