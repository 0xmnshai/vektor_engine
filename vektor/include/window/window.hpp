#pragma once

#ifdef __APPLE__
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/core.hpp"
#include "events/event.hpp"
#include "renderer/graphics_context.hpp"

namespace vektor::window
{
    struct WindowProps
    {
        std::string title;
        unsigned int width, height;

        WindowProps(const std::string &title = "Vektor Engine",
                    uint32_t width = WINDOW_WIDTH,
                    uint32_t height = WINDOW_HEIGHT) : title(title), width(width), height(height) {}

        bool operator==(const WindowProps &other) const
        {
            return title == other.title && width == other.width && height == other.height;
        }

        bool operator!=(const WindowProps &other) const
        {
            return !(*this == other);
        }
    };

    class VEKTOR_API Window
    {
    public:
        using EventCallbackFn = std::function<void(event::Event &)>;

        virtual ~Window() {}

        virtual void onUpdate() = 0;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;

        virtual void setEventCallback(const EventCallbackFn &callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;

        virtual GLFWwindow *getNativeWindow() const = 0;

        static Window *create(const WindowProps &props = WindowProps());

        static void setWindowIcon(const std::string &iconPath);
        static void setWindowTitle(const std::string &title);
        static void setWindowPosition(int xpos, int ypos);

        static void setGLFWWindowHints();

    private:
        virtual void init(const WindowProps &props) = 0;
        virtual void shutdown() = 0;
        virtual void pollEvents() = 0;

        virtual void *getGLFWWindow() const = 0;

        struct WindowData
        {
            std::string title;
            unsigned int width, height;
            bool vsync;
            GLFWwindow *glfwWindow;
            EventCallbackFn eventCallback;
        };

        static WindowData *getWindowData(GLFWwindow *window);

        static void setWindowData(GLFWwindow *window, const WindowData &data);

    protected:
        WindowData m_data;

        renderer::GraphicsContext *m_Context;

        static void setGLFWCallbacks(GLFWwindow *window);
    };
}

// static void GLFWErrorCallback(int error, const char *description);
// static void GLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
// static void GLFWCharCallback(GLFWwindow *window, unsigned int codepoint);
// static void GLFWMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
// static void GLFWCursorPosCallback(GLFWwindow *window, double xpos, double ypos);
// static void GLFWScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
// static void GLFWWindowResizeCallback(GLFWwindow *window, int width, int height);
// static void GLFWWindowCloseCallback(GLFWwindow *window);
// static void GLFWWindowFocusCallback(GLFWwindow *window, int focused);
// static void GLFWWindowIconifyCallback(GLFWwindow *window, int iconified);
// static void GLFWWindowMaximizeCallback(GLFWwindow *window, int maximized);
// static void GLFWWindowRefreshCallback(GLFWwindow *window);
// static void GLFWWindowFramebufferSizeCallback(GLFWwindow *window, int width, int height);
// static void GLFWWindowContentScaleCallback(GLFWwindow *window, float xscale, float yscale);
// static void GLFWWindowPosCallback(GLFWwindow *window, int xpos, int ypos);
// static void GLFWMonitorCallback(GLFWmonitor *monitor, int event);
// static void GLFWDropCallback(GLFWwindow *window, int count, const char **paths);
// static void setGLFWWindowIcon(GLFWwindow *window, const std::string &iconPath);
// static void setGLFWWindowTitle(GLFWwindow *window, const std::string &title);
// static void setGLFWWindowPosition(GLFWwindow *window, int xpos, int ypos);
// static void setGLFWWindowOpacity(GLFWwindow *window, double opacity);
// static void setGLFWWindowSize(GLFWwindow *window, int width, int height);
// static void setGLFWWindowSizeLimits(GLFWwindow *window, int minWidth, int minHeight, int maxWidth, int maxHeight);
// static void setGLFWWindowAspectRatio(GLFWwindow *window, int numer, int denom);
// static void setGLFWWindowMonitor(GLFWwindow *window, GLFWmonitor *monitor, int xpos, int ypos, int width, int height, int refreshRate);
// static void setGLFWWindowContentScale(GLFWwindow *window, float xscale, float yscale);
// static void setGLFWWindowUserPointer(GLFWwindow *window, void *pointer);
// static void setGLFWWindowShouldClose(GLFWwindow *window, bool value);
// static bool getGLFWWindowShouldClose(GLFWwindow *window);
// static void setGLFWWindowAttrib(GLFWwindow *window, int attrib, int value);
// static int getGLFWWindowAttrib(GLFWwindow *window, int attrib);
// static void setGLFWWindowPos(GLFWwindow *window, int xpos, int ypos);
// static void setGLFWWindowSize(GLFWwindow *window, int width, int height);
// static void setGLFWWindowSizeLimits(GLFWwindow *window, int minWidth, int minHeight, int maxWidth, int maxHeight);
// static void setGLFWWindowAspectRatio(GLFWwindow *window, int numer, int denom);
// static void setGLFWWindowMonitor(GLFWwindow *window, GLFWmonitor *monitor, int xpos, int ypos, int width, int height, int refreshRate);
// static void setGLFWWindowContentScale(GLFWwindow *window, float xscale, float yscale);
// static void setGLFWWindowUserPointer(GLFWwindow *window, void *pointer);
// static void setGLFWWindowShouldClose(GLFWwindow *window, bool value);
// static bool getGLFWWindowShouldClose(GLFWwindow *window);
// static void setGLFWWindowAttrib(GLFWwindow *window, int attrib, int value);
// static int getGLFWWindowAttrib(GLFWwindow *window, int attrib);
// static void setGLFWWindowPos(GLFWwindow *window, int xpos, int ypos);
// static void setGLFWWindowSize(GLFWwindow *window, int width, int height);
// static void setGLFWWindowSizeLimits(GLFWwindow *window, int minWidth, int minHeight, int maxWidth, int maxHeight);
// static void setGLFWWindowAspectRatio(GLFWwindow *window, int numer, int denom);
// static void setGLFWWindowMonitor(GLFWwindow *window, GLFWmonitor *monitor, int xpos, int ypos, int width, int height, int refreshRate);
// static void setGLFWWindowContentScale(GLFWwindow *window, float xscale, float yscale);
// static void setGLFWWindowUserPointer(GLFWwindow *window, void *pointer);
// static void setGLFWWindowShouldClose(GLFWwindow *window, bool value);
// static bool getGLFWWindowShouldClose(GLFWwindow *window);
// static void setGLFWWindowAttrib(GLFWwindow *window, int attrib, int value);
