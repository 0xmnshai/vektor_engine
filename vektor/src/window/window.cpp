
#include <format>
#include <fstream>
#include <filesystem>

#include "logger/logger.hpp"
#include "window/window.hpp"

// events 
#include "events/window.hpp"
#include "events/mouse.hpp"
#include "events/keyboard.hpp"

namespace vektor::window
{
    static bool s_GLFWInitialized = false;

    static GLFWwindow *s_MainWindowHandle = nullptr;

    static void GLFWErrorCallback(int error, const char *description)
    {
        VEKTOR_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    class GlfwWindow : public Window
    {
    public:
        explicit GlfwWindow(const WindowProps &props)
        {
            init(props);
        }

        virtual ~GlfwWindow() override
        {
            shutdown();
        }

        void onUpdate() override
        {
            glfwPollEvents();

            if (m_data.width > 0 && m_data.height > 0)
            {
                glfwSwapBuffers(m_data.glfwWindow);
            }
        }
        [[nodiscard]] unsigned int getWidth() const override { return m_data.width; }
        [[nodiscard]] unsigned int getHeight() const override { return m_data.height; }

        void setEventCallback(const EventCallbackFn &callback) override
        {
            VEKTOR_INFO("Setting event callback");
            m_data.eventCallback = callback;
        }

        void setVSync(bool enabled) override
        {
            if (enabled)
                glfwSwapInterval(1);
            else
                glfwSwapInterval(0);

            m_data.vsync = enabled;
            glfwSwapInterval(enabled ? 1 : 0);
        }

        bool isVSync() const override
        {
            return m_data.vsync;
        }

        void *getNativeWindow() const override
        {
            return m_data.glfwWindow;
        }

        void pollEvents() override
        {
            glfwPollEvents();
        }

    private:
        virtual void *getGLFWWindow() const override { return m_data.glfwWindow; }

        void init(const WindowProps &props) override
        {
            m_data.title = props.title;
            m_data.width = props.width;
            m_data.height = props.height;

            VEKTOR_CORE_INFO("Creating window: {} ({}, {})", props.title, props.width, props.height);

            if (!s_GLFWInitialized)
            {
                const int success = glfwInit();
                if (!success)
                {
                    VEKTOR_CORE_ASSERT(success, "Could not initialize GLFW!");
                    std::terminate();
                }

                glfwSetErrorCallback(GLFWErrorCallback);
                s_GLFWInitialized = true;
            }

            Window::setGLFWWindowHints();
            VEKTOR_CORE_INFO("GLFW Window hints set");

            m_data.glfwWindow = glfwCreateWindow(
                static_cast<int>(m_data.width),
                static_cast<int>(m_data.height),
                m_data.title.c_str(),
                nullptr,
                nullptr);

            if (!m_data.glfwWindow)
            {
                VEKTOR_CORE_ERROR("Failed to create GLFW window! (Check OpenGL version requirements) !");
                glfwTerminate();
                std::terminate();
            }

            if (!s_MainWindowHandle)
            {
                s_MainWindowHandle = m_data.glfwWindow;
            }

            glfwMakeContextCurrent(m_data.glfwWindow);

            glfwSetWindowUserPointer(m_data.glfwWindow, &m_data);

            setVSync(true);

            Window::setGLFWCallbacks(m_data.glfwWindow);
        }

        void shutdown() override
        {
            glfwDestroyWindow(m_data.glfwWindow);
            glfwTerminate();
        }
    };

    Window *Window::create(const WindowProps &props)
    {
        return new GlfwWindow(props);
    }

    void Window::setWindowTitle(const std::string &title)
    {
        if (s_MainWindowHandle)
        {
            glfwSetWindowTitle(s_MainWindowHandle, title.c_str());
        }
    }

    void Window::setWindowPosition(int xpos, int ypos)
    {
        if (s_MainWindowHandle)
        {
            glfwSetWindowPos(s_MainWindowHandle, xpos, ypos);
        }
    }

    void Window::setWindowIcon(const std::string &iconPath)
    {
        if (!s_MainWindowHandle)
            return;

        /*
        GLFWimage images[1];
        int width, height, channels;
        unsigned char* pixels = stbi_load(iconPath.c_str(), &width, &height, &channels, 4);

        if (pixels) {
            images[0].width = width;
            images[0].height = height;
            images[0].pixels = pixels;
            glfwSetWindowIcon(s_MainWindowHandle, 1, images);
            stbi_image_free(pixels);
        } else {
            std::cerr << std::format("Failed to load icon: {}\n", iconPath);
        }
        */
    }

    void Window::setGLFWWindowHints()
    {
#ifdef __APPLE__
        // macOS supports up to OpenGL 4.1
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
#else
        // Windows/Linux support 4.6
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    }

    Window::WindowData *Window::getWindowData(GLFWwindow *window)
    {
        return static_cast<WindowData *>(glfwGetWindowUserPointer(window));
    }

    void Window::setWindowData(GLFWwindow *window, const WindowData &data)
    {
        WindowData *ptr = getWindowData(window);
        if (ptr)
            *ptr = data;
    }

    void Window::setGLFWCallbacks(GLFWwindow *window)
    {

        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *win, int width, int height)
                                       {

            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(win));
            if (!data) return; // Safety check

            data->width = width;
            data->height = height;

            // Prevent divide-by-zero crashes on minimize/transition
            if (width <= 0 || height <= 0) return;

            event::WindowResizeEvent event(width, height);
            if(data->eventCallback) data->eventCallback(event); });

        glfwSetWindowCloseCallback(window, [](GLFWwindow *win)
                                   {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(win));
            event::WindowCloseEvent event;
            if(data.eventCallback) data.eventCallback(event); });

        glfwSetKeyCallback(window, [](GLFWwindow *win, int key, int scancode, int action, int mods)
                           {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(win));
            switch (action)
            {
                case GLFW_PRESS:
                {
                    event::KeyPressedEvent event(key, 0);
                    if(data.eventCallback) data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    event::KeyReleasedEvent event(key);
                    if(data.eventCallback) data.eventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    event::KeyPressedEvent event(key, 1);
                    if(data.eventCallback) data.eventCallback(event);
                    break;
                }
            } });

        glfwSetMouseButtonCallback(window, [](GLFWwindow *win, int button, int action, int mods)
                                   {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(win));
            switch (action)
            {
                case GLFW_PRESS:
                {
                    event::MouseButtonPressedEvent event(button);
                    if(data.eventCallback) data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    event::MouseButtonReleasedEvent event(button);
                    if(data.eventCallback) data.eventCallback(event);
                    break;
                }
            } });

        glfwSetScrollCallback(window, [](GLFWwindow *win, double xOffset, double yOffset)
                              {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(win));
            event::MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            if(data.eventCallback) data.eventCallback(event); });

        glfwSetCursorPosCallback(window, [](GLFWwindow *win, double xPos, double yPos)
                                 {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(win));
            event::MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
            if(data.eventCallback) data.eventCallback(event); });

        glfwSetCharCallback(window, [](GLFWwindow *win, unsigned int keycode)
                            {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(win));
            event::KeyTypedEvent event(keycode);
            if(data.eventCallback) data.eventCallback(event); });
    }
}