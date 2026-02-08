
#include "application/application.hpp"
#include "input/window.hpp"


namespace vektor::input::window
{
    bool input::window::Input::isKeyPressedImpl(int keycode)
    {
        GLFWwindow *window = static_cast<GLFWwindow *>(Application::getInstance().getWindow().getNativeWindow());
        int state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool input::window::Input::isMouseButtonPressedImpl(int button)
    {
        GLFWwindow *window = static_cast<GLFWwindow *>(Application::getInstance().getWindow().getNativeWindow());
        int state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    float input::window::Input::getMouseXImpl()
    {
        glm::vec2 pos = getMousePositionImpl();
        return static_cast<float>(pos.x);
    }

    float input::window::Input::getMouseYImpl()
    {
        glm::vec2 pos = getMousePositionImpl();
        return static_cast<float>(pos.y);
    }

    glm::vec2 input::window::Input::getMousePositionImpl()
    {
        GLFWwindow *window = static_cast<GLFWwindow *>(Application::getInstance().getWindow().getNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}