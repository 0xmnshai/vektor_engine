#pragma once

#include "vk_pch.hpp"
#include "core/core.hpp"
#include "events/event.hpp"

namespace vektor::event
{
    class VEKTOR_API MouseEvent : public Event
    {
    public:
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    };

    class VEKTOR_API MouseMovedEvent : public MouseEvent
    {
    public:
        MouseMovedEvent(float x, float y)
            : m_MouseX(x), m_MouseY(y) {}

        inline float getX() const { return m_MouseX; }
        inline float getY() const { return m_MouseY; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
    private:
        float m_MouseX, m_MouseY;
    };

    class VEKTOR_API MouseScrolledEvent : public MouseEvent
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset) {}

        inline float getXOffset() const { return m_XOffset; }
        inline float getYOffset() const { return m_YOffset; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
    private:
        float m_XOffset, m_YOffset;
    };

    class VEKTOR_API MouseButtonEvent : public MouseEvent
    {
    public:
        inline int getMouseButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)
    protected:
        MouseButtonEvent(int button)
            : m_Button(button) {}

        int m_Button;
    };

    class VEKTOR_API MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button)
            : MouseButtonEvent(button) {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class VEKTOR_API MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button)
            : MouseButtonEvent(button) {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}