#pragma once

#include "core/core.hpp"

namespace vektor::renderer
{
    enum class API
    {
        NONE = 0,
        OPENGL = 1
    };

    class VEKTOR_API Renderer
    {
    public:
        inline static API getAPI() { return s_RendererAPI; }

    private:
        static API s_RendererAPI;
    };
}