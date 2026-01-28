#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/core.hpp"
#include "renderer/graphics_context.hpp"

namespace vektor::renderer
{
    class VEKTOR_API OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow *windowHandle); //  : m_WindowHandle(windowHandle) {}
       
        virtual void init() override;
        virtual void swapBuffers() override;
        virtual void shutdown() override;

    private:
        GLFWwindow *m_WindowHandle;
    };
}