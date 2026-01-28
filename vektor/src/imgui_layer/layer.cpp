
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "core/core.hpp"
#include "imgui/layer.hpp"
#include "events/event.hpp"
#include "application/application.hpp"
#include "imgui/layer.hpp"

namespace vektor::imgui_layer
{
    Layer::Layer() : layer::Layer("ImGuiLayer")
    {
    }

    imgui_layer::Layer::~Layer()
    {
    }

    void imgui_layer::Layer::onAttach()
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        ImGuiStyle &style = ImGui::GetStyle();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application &app = Application::getInstance();
        GLFWwindow *window = static_cast<GLFWwindow *>(app.getWindow().getNativeWindow());

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void imgui_layer::Layer::onDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void imgui_layer::Layer::begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui::DockSpaceOverViewport();
    }

    void imgui_layer::Layer::onRender()
    {
        static bool show_demo = false;
        ImGui::ShowDemoWindow(&show_demo);
    }

    void imgui_layer::Layer::end()
    {
        ImGuiIO &io = ImGui::GetIO();
        Application &app = Application::getInstance();

        int w = app.getWindow().getWidth();
        int h = app.getWindow().getHeight();

        io.DisplaySize = ImVec2(w, h);

        // begin
        // ImGui_ImplOpenGL3_NewFrame();
        // ImGui_ImplGlfw_NewFrame();
        // ImGui::NewFrame();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            // ImGui::UpdatePlatformWindows();
            // ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        // end
        glViewport(0, 0, w, h);
    }

    void imgui_layer::Layer::onEvent(event::Event &event)
    {
        event::EventDispatcher dispatcher(event);

        dispatcher.dispatch<event::MouseButtonPressedEvent>(VEKTOR_BIND_EVENT_FN(imgui_layer::Layer::onMouseButtonPressedEvent));
        dispatcher.dispatch<event::MouseButtonReleasedEvent>(VEKTOR_BIND_EVENT_FN(imgui_layer::Layer::onMouseButtonReleasedEvent));
        dispatcher.dispatch<event::MouseMovedEvent>(VEKTOR_BIND_EVENT_FN(imgui_layer::Layer::onMouseMovedEvent));
        dispatcher.dispatch<event::MouseScrolledEvent>(VEKTOR_BIND_EVENT_FN(imgui_layer::Layer::onMouseScrolledEvent));

        dispatcher.dispatch<event::KeyPressedEvent>(VEKTOR_BIND_EVENT_FN(imgui_layer::Layer::onKeyPressedEvent));
        dispatcher.dispatch<event::KeyReleasedEvent>(VEKTOR_BIND_EVENT_FN(imgui_layer::Layer::onKeyReleasedEvent));
        dispatcher.dispatch<event::KeyTypedEvent>(VEKTOR_BIND_EVENT_FN(imgui_layer::Layer::onKeyTypedEvent));

        dispatcher.dispatch<event::WindowResizeEvent>(VEKTOR_BIND_EVENT_FN(imgui_layer::Layer::onWindowResizeEvent));
        dispatcher.dispatch<event::WindowMovedEvent>(VEKTOR_BIND_EVENT_FN(imgui_layer::Layer::onWindowMovedEvent));
        dispatcher.dispatch<event::WindowFocusEvent>(VEKTOR_BIND_EVENT_FN(imgui_layer::Layer::onWindowFocusEvent));
        dispatcher.dispatch<event::WindowLostFocusEvent>(VEKTOR_BIND_EVENT_FN(imgui_layer::Layer::onWindowLostFocusEvent));

        dispatcher.dispatch<event::AppTickEvent>(VEKTOR_BIND_EVENT_FN(imgui_layer::Layer::onAppTickEvent));
        dispatcher.dispatch<event::AppUpdateEvent>(VEKTOR_BIND_EVENT_FN(imgui_layer::Layer::onAppUpdateEvent));
        dispatcher.dispatch<event::AppRenderEvent>(VEKTOR_BIND_EVENT_FN(imgui_layer::Layer::onAppRenderEvent));
    }

    // Mouse events
    bool imgui_layer::Layer::onMouseButtonPressedEvent(event::MouseButtonPressedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[event.getMouseButton()] = true;
        return true;
    }

    bool imgui_layer::Layer::onMouseButtonReleasedEvent(event::MouseButtonReleasedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[event.getMouseButton()] = false;
        return true;
    }

    bool imgui_layer::Layer::onMouseMovedEvent(event::MouseMovedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MousePos = ImVec2(event.getX(), event.getY());
        return true;
    }

    bool imgui_layer::Layer::onMouseScrolledEvent(event::MouseScrolledEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseWheel += event.getYOffset();
        io.MouseWheelH += event.getXOffset();
        return true;
    }

    // Keyboard events
    bool imgui_layer::Layer::onKeyPressedEvent(event::KeyPressedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.AddKeyEvent(ImGuiKey_LeftCtrl, ImGui::IsKeyDown(ImGuiKey_LeftCtrl));
        io.AddKeyEvent(ImGuiKey_RightCtrl, ImGui::IsKeyDown(ImGuiKey_RightCtrl));
        io.AddKeyEvent(ImGuiKey_LeftShift, ImGui::IsKeyDown(ImGuiKey_LeftShift));
        io.AddKeyEvent(ImGuiKey_RightShift, ImGui::IsKeyDown(ImGuiKey_RightShift));
        return true;
    }

    bool imgui_layer::Layer::onKeyReleasedEvent(event::KeyReleasedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.AddKeyEvent(ImGuiKey_LeftCtrl, false);
        io.AddKeyEvent(ImGuiKey_RightCtrl, false);
        io.AddKeyEvent(ImGuiKey_LeftShift, false);
        io.AddKeyEvent(ImGuiKey_RightShift, false);
        return true;
    }

    bool imgui_layer::Layer::onKeyTypedEvent(event::KeyTypedEvent &event)
    {
        return false;
        //     ImGuiIO &io = ImGui::GetIO();
        //     io.AddInputCharacter(event.getKeyCode());
        //     return true;
    }

    // Window events
    bool imgui_layer::Layer::onWindowResizeEvent(event::WindowResizeEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(event.getWidth(), event.getHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, event.getWidth(), event.getHeight());
        return false;
    }

    bool imgui_layer::Layer::onWindowMovedEvent(event::WindowMovedEvent &event)
    {
        return false;
    }

    bool imgui_layer::Layer::onWindowFocusEvent(event::WindowFocusEvent &event)
    {
        return false;
    }

    bool imgui_layer::Layer::onWindowLostFocusEvent(event::WindowLostFocusEvent &event)
    {
        return false;
    }

    // App events
    bool imgui_layer::Layer::onAppTickEvent(event::AppTickEvent &event)
    {
        return false;
    }

    bool imgui_layer::Layer::onAppUpdateEvent(event::AppUpdateEvent &event)
    {
        return false;
    }

    bool imgui_layer::Layer::onAppRenderEvent(event::AppRenderEvent &event)
    {
        return false;
    }

}