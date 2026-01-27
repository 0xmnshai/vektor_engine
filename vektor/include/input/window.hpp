#pragma once

#include <glm/glm.hpp>  

#include "input/input.hpp"

namespace vektor::input::window
{
    class Input : public input::Input
    {
    protected:
        virtual bool isKeyPressedImpl(int keyCode) override;

        virtual bool isMouseButtonPressedImpl(int button) override;

        virtual float getMouseXImpl() override;
        virtual float getMouseYImpl() override;

        virtual glm::vec2 getMousePositionImpl() override;
    };
}