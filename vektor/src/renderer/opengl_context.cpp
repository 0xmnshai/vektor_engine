
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "logger/logger.hpp"
#include "renderer/opengl_context.hpp"

namespace vektor::renderer
{
    OpenGLContext::OpenGLContext(GLFWwindow *windowHandle)
        : m_WindowHandle(windowHandle)
    {
        VEKTOR_CORE_ASSERT(windowHandle, "Window handle is null!")
    }

    void OpenGLContext::init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        glfwSwapInterval(1);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        VEKTOR_CORE_INFO("OpenGL context initialized successfully");
    }

    void OpenGLContext::swapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

    void OpenGLContext::shutdown()
    {
    }
}
