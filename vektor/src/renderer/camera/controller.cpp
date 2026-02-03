
#include "core/timestep.hpp"

#include "events/event.hpp"
#include "events/mouse.hpp"
#include "events/window.hpp"
#include "events/keycode.hpp"

#include "input/input.hpp"

#include "renderer/camera/controller.hpp"

namespace vektor::renderer::camera
{

    Controller::Controller(float aspectRatio, float zoomLevel, bool rotation)
        : m_AspectRatio(aspectRatio),
          m_ZoomLevel(zoomLevel),
          m_Rotation(rotation)
    {
        m_Camera = std::make_shared<Orthographic>(
            -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    void Controller::onUpdate(core::Timestep timestep)
    {
        using vektor::KeyCode;

        if (input::Input::isKeyPressed(KeyCode::A) ||
            input::Input::isKeyPressed(KeyCode::Left))
            m_CameraPosition.x -= m_CameraTranslationSpeed * timestep;

        if (input::Input::isKeyPressed(KeyCode::D) ||
            input::Input::isKeyPressed(KeyCode::Right))
            m_CameraPosition.x += m_CameraTranslationSpeed * timestep;

        if (input::Input::isKeyPressed(KeyCode::W) ||
            input::Input::isKeyPressed(KeyCode::Up))
            m_CameraPosition.y += m_CameraTranslationSpeed * timestep;

        if (input::Input::isKeyPressed(KeyCode::S) ||
            input::Input::isKeyPressed(KeyCode::Down))
            m_CameraPosition.y -= m_CameraTranslationSpeed * timestep;

        if (m_Rotation)
        {
            if (input::Input::isKeyPressed(KeyCode::R))
                m_CameraRotation += m_CameraRotationSpeed * timestep;

            if (input::Input::isKeyPressed(KeyCode::T))
                m_CameraRotation -= m_CameraRotationSpeed * timestep;

            m_Camera->setRotation(m_CameraRotation);
        }

        m_Camera->setPosition(m_CameraPosition);
    }

    void Controller::onEvent(event::Event &event)
    {
        event::EventDispatcher dispatcher(event);
        dispatcher.dispatch<event::MouseScrolledEvent>(VEKTOR_BIND_EVENT_FN(Controller::onMouseScrolled));
        dispatcher.dispatch<event::WindowResizeEvent>(VEKTOR_BIND_EVENT_FN(Controller::onWindowResize));
    }

    bool Controller::onMouseScrolled(const event::MouseScrolledEvent &event)
    {
        m_ZoomLevel -= event.getYOffset() * 0.1f;
        m_Camera->setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }
    bool Controller::onWindowResize(const event::WindowResizeEvent &event)
    {
        m_AspectRatio = static_cast<float>(event.getWidth()) / static_cast<float>(event.getHeight());
        m_Camera->setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }
}