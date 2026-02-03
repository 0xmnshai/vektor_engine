#pragma once

#include <glm/glm.hpp>

#include "core/core.hpp"
#include "events/keycode.hpp"

namespace vektor::input
{
    class VEKTOR_API Input
    {
    public:
        static bool isKeyPressed(vektor::KeyCode key)
        {
            return s_Instance->isKeyPressedImpl(static_cast<int>(key));
        }

        static bool isMouseButtonPressed(vektor::MouseCode button)
        {
            return s_Instance->isMouseButtonPressedImpl(static_cast<int>(button));
        }

        static float getMouseX() { return s_Instance->getMouseXImpl(); }
        static float getMouseY() { return s_Instance->getMouseYImpl(); }
        static glm::vec2 getMousePosition() { return s_Instance->getMousePositionImpl(); }

    protected:
        virtual bool isKeyPressedImpl(int keycode) = 0;
        virtual bool isMouseButtonPressedImpl(int button) = 0;

        virtual float getMouseXImpl() = 0;
        virtual float getMouseYImpl() = 0;
        virtual glm::vec2 getMousePositionImpl() = 0;

    private:
        static Input *s_Instance;
    };
}
