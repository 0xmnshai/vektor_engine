#pragma once

#include "core/core.hpp"

namespace vektor::renderer
{
    class VEKTOR_API GraphicsContext
    {
    public:
        virtual void init() = 0;
        virtual void swapBuffers() = 0;
        virtual void shutdown() = 0;
    };
}