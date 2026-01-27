#pragma once

#include <glm/glm.hpp>

#include "core/core.hpp"

namespace vektor::input
{
    class VEKTOR_API Input
    {
    public:
        inline static bool isKeyPressed(int keycode) { return s_Instance->isKeyPressedImpl(keycode); }
        inline static bool isMouseButtonPressed(int button) { return s_Instance->isMouseButtonPressedImpl(button); }

        inline static float getMouseX() { return s_Instance->getMouseXImpl(); }
        inline static float getMouseY() { return s_Instance->getMouseYImpl(); }
        inline static glm::vec2 getMousePosition() { return s_Instance->getMousePositionImpl(); }

    protected:
        virtual bool isKeyPressedImpl(int keycode) = 0;        // { return s_Instance->isKeyPressedImpl(keycode); }
        virtual bool isMouseButtonPressedImpl(int button) = 0; // { return s_Instance->isMouseButtonPressedImpl(button); }

        virtual float getMouseXImpl() = 0;
        virtual float getMouseYImpl() = 0;
        virtual glm::vec2 getMousePositionImpl() = 0;

    private:
        static Input *s_Instance;
    };
}