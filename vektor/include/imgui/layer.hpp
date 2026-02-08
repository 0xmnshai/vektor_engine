#pragma once

#include "core/core.hpp"
#include "layer/layer.hpp"

#include "events/event.hpp"
#include "events/mouse.hpp"
#include "events/keyboard.hpp"
#include "events/window.hpp"

namespace vektor::imgui
{
    class VEKTOR_API Layer : public layer::Layer
    {
    public:
        Layer();
        ~Layer();

        void onAttach() override;
        void onDetach() override;

        void onRender() override;

        void onEvent(event::Event &event) override;
        void blockEvents(bool block) { m_BlockEvents = block; }

        void begin();
        void end();

        // testing theme
        void setTheme();

    private:
         bool m_BlockEvents = true;
        float m_Time = 0.0f;

        // Mouse events
        bool onMouseButtonPressedEvent(event::MouseButtonPressedEvent &event);
        bool onMouseButtonReleasedEvent(event::MouseButtonReleasedEvent &event);
        bool onMouseMovedEvent(event::MouseMovedEvent &event);
        bool onMouseScrolledEvent(event::MouseScrolledEvent &event);

        // Keyboard events
        bool onKeyPressedEvent(event::KeyPressedEvent &event);
        bool onKeyReleasedEvent(event::KeyReleasedEvent &event);
        bool onKeyTypedEvent(event::KeyTypedEvent &event);

        // Window events
        bool onWindowResizeEvent(event::WindowResizeEvent &event);
        bool onWindowMovedEvent(event::WindowMovedEvent &event);
        bool onWindowFocusEvent(event::WindowFocusEvent &event);
        bool onWindowLostFocusEvent(event::WindowLostFocusEvent &event);

        // App events
        bool onAppTickEvent(event::AppTickEvent &event);
        bool onAppUpdateEvent(event::AppUpdateEvent &event);
        bool onAppRenderEvent(event::AppRenderEvent &event);
    };
}