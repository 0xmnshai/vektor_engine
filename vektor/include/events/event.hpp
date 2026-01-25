#pragma once

#include "vk_pch.hpp"
#include "core/core.hpp"

namespace vektor::event
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,

        AppTick,
        AppUpdate,
        AppRender,

        KeyPressed,
        KeyReleased,
        KeyTyped,

        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

#define EVENT_CLASS_TYPE(type)                                                  \
    static EventType getStaticType() { return EventType::type; }                \
    virtual EventType getEventType() const override { return getStaticType(); } \
    virtual const char *getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual int getCategoryFlags() const override { return category; }

    class VEKTOR_API Event
    {
    public:
        virtual ~Event() = default;

        virtual EventType getEventType() const = 0;
        virtual const char *getName() const = 0;
        virtual int getCategoryFlags() const = 0;
        virtual std::string toString() const { return getName(); }

        bool isInCategory(EventCategory category)
        {
            return getCategoryFlags() & category;
        }

        bool isHandled() const { return handled; }
        void setHandled(bool handled) { this->handled = handled; }

    private:
        bool handled = false;
    };

    class EventDispatcher
    {
    public:
        EventDispatcher(Event &event)
            : m_event(event)
        {
        }

        template <typename T, typename F>
            requires std::is_base_of_v<Event, T>
        bool dispatch(const F &func)
        {
            if (m_event.getEventType() == T::getStaticType())
            {
                m_event.setHandled(func(static_cast<T &>(m_event)));
                return true;
            }
            return false;
        }

    private:
        Event &m_event;
    };

    inline std::ostream &operator<<(std::ostream &os, const Event &e)
    {
        return os << e.toString();
    }
}