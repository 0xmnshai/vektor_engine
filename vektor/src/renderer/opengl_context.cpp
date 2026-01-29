
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
        glEnable(GL_DEPTH_TEST);

        std::string gl_version = std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION)));                                // GluByte*
        std::string shading_language_version = std::string(reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION))); // GluByte*
        VEKTOR_CORE_INFO("OpenGL Version: {0}", gl_version);
        VEKTOR_CORE_INFO("GLSL Version: {0}", shading_language_version);

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
