#include <imgui_internal.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include "core/core.hpp"
#include "imgui/layer.hpp"
#include "events/event.hpp"
#include "application/application.hpp"

namespace vektor::imgui
{
    Layer::Layer() : layer::Layer("ImGuiLayer")
    {
    }

    imgui::Layer::~Layer()
    {
    }

    void imgui::Layer::setTheme()
    {
        ImGuiStyle &style = ImGui::GetStyle();
        ImVec4 *colors = style.Colors;

        // --- Metrics (Kept exactly as requested for the "Stacked" look) ---
        style.WindowPadding = ImVec2(6.0f, 6.0f);
        style.FramePadding = ImVec2(5.0f, 2.0f);
        style.ItemSpacing = ImVec2(8.0f, 4.0f);
        style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
        style.IndentSpacing = 22.0f;
        style.ScrollbarSize = 15.0f;
        style.GrabMinSize = 12.0f;

        // --- Rounding ---
        style.WindowRounding = 0.0f;
        style.ChildRounding = 0.0f;
        style.FrameRounding = 2.0f;
        style.PopupRounding = 0.0f;
        style.ScrollbarRounding = 9.0f;
        style.GrabRounding = 2.0f;
        style.TabRounding = 0.0f;

        // --- Borders ---
        style.WindowBorderSize = 1.0f;
        style.ChildBorderSize = 1.0f;
        style.PopupBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;
        style.TabBorderSize = 0.0f;

        // --- Monochrome Color Palette ---

        // Core Greys
        ImVec4 kBackground = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        ImVec4 kInputBg = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); // Darker inset
        ImVec4 kBorder = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);

        // Accents (Greyscale)
        ImVec4 kSelection = ImVec4(0.30f, 0.30f, 0.30f, 1.00f); // Selected Item Background
        ImVec4 kHover = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);     // Hovered Item Background
        ImVec4 kActive = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);    // Active Item/Click
        ImVec4 kWhite = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);     // High contrast text/checks

        // Text
        colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

        // Windows
        colors[ImGuiCol_WindowBg] = kBackground;
        colors[ImGuiCol_ChildBg] = kBackground;
        colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f); // Slightly darker for popups

        // Borders
        colors[ImGuiCol_Border] = kBorder;
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Inputs (Frame)
        colors[ImGuiCol_FrameBg] = kInputBg;
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = kInputBg;

        // Title Bar
        colors[ImGuiCol_TitleBg] = kInputBg;
        colors[ImGuiCol_TitleBgActive] = kInputBg;
        colors[ImGuiCol_TitleBgCollapsed] = kInputBg;
        colors[ImGuiCol_MenuBarBg] = kInputBg;

        // Scrollbar
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);

        // Checkmark (Bright White)
        colors[ImGuiCol_CheckMark] = kWhite;

        // Sliders / Drags
        colors[ImGuiCol_SliderGrab] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f); // Light grey knob
        colors[ImGuiCol_SliderGrabActive] = kWhite;

        // Buttons
        colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

        // Headers (Hierarchy Selection) - Replaced Blue with Grey
        colors[ImGuiCol_Header] = kSelection;
        colors[ImGuiCol_HeaderHovered] = kHover;
        colors[ImGuiCol_HeaderActive] = kActive;

        // Separators
        colors[ImGuiCol_Separator] = kBorder;
        colors[ImGuiCol_SeparatorHovered] = kHover;
        colors[ImGuiCol_SeparatorActive] = kActive;

        // Resize Grip
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_TabActive] = kBackground;
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = kBackground;

        // Docking - Monochrome preview
        colors[ImGuiCol_DockingPreview] = ImVec4(0.60f, 0.60f, 0.60f, 0.30f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

        // Plot
        colors[ImGuiCol_PlotLines] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = kWhite;
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = kWhite;

        // Table
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.03f);

        // Navigation
        colors[ImGuiCol_NavHighlight] = kWhite;
        colors[ImGuiCol_NavWindowingHighlight] = kWhite;
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
    }

    void imgui::Layer::onAttach()
    {
        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // --- Font Configuration ---
        // To truly match the screenshot's clean look, a slightly larger font is often better.
        // Default ImGui font is a bit pixelated.
        float fontSize = 14.0f;
        ImFontConfig config;
        config.OversampleH = 3;
        config.OversampleV = 3;
        io.Fonts->AddFontDefault(&config);

        setTheme();

        Application &app = Application::getInstance();
        GLFWwindow *window = static_cast<GLFWwindow *>(app.getWindow().getNativeWindow());

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void imgui::Layer::onDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void imgui::Layer::begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;

        const ImGuiViewport *viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", nullptr, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        ImGui::End();
    }

    void imgui::Layer::onRender()
    {
        static bool show_demo = true;
        if (show_demo)
            ImGui::ShowDemoWindow(&show_demo);
    }

    void imgui::Layer::end()
    {
        ImGuiIO &io = ImGui::GetIO();
        Application &app = Application::getInstance();

        float w = static_cast<float>(app.getWindow().getWidth());
        float h = static_cast<float>(app.getWindow().getHeight());

        io.DisplaySize = ImVec2(w, h);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            glfwMakeContextCurrent(backup_current_context);
        }

        glViewport(0, 0, static_cast<int>(w), static_cast<int>(h));
    }

    void imgui::Layer::onEvent(event::Event &event)
    {
        event::EventDispatcher dispatcher(event);

        dispatcher.dispatch<event::MouseButtonPressedEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onMouseButtonPressedEvent));
        dispatcher.dispatch<event::MouseButtonReleasedEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onMouseButtonReleasedEvent));
        dispatcher.dispatch<event::MouseMovedEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onMouseMovedEvent));
        dispatcher.dispatch<event::MouseScrolledEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onMouseScrolledEvent));

        dispatcher.dispatch<event::KeyPressedEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onKeyPressedEvent));
        dispatcher.dispatch<event::KeyReleasedEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onKeyReleasedEvent));
        dispatcher.dispatch<event::KeyTypedEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onKeyTypedEvent));

        dispatcher.dispatch<event::WindowResizeEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onWindowResizeEvent));
        dispatcher.dispatch<event::WindowMovedEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onWindowMovedEvent));
        dispatcher.dispatch<event::WindowFocusEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onWindowFocusEvent));
        dispatcher.dispatch<event::WindowLostFocusEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onWindowLostFocusEvent));

        dispatcher.dispatch<event::AppTickEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onAppTickEvent));
        dispatcher.dispatch<event::AppUpdateEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onAppUpdateEvent));
        dispatcher.dispatch<event::AppRenderEvent>(VEKTOR_BIND_EVENT_FN(imgui::Layer::onAppRenderEvent));
    }

    // Mouse events
    bool imgui::Layer::onMouseButtonPressedEvent(event::MouseButtonPressedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[event.getMouseButton()] = true;
        return false;
    }

    bool imgui::Layer::onMouseButtonReleasedEvent(event::MouseButtonReleasedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[event.getMouseButton()] = false;
        return false;
    }

    bool imgui::Layer::onMouseMovedEvent(event::MouseMovedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MousePos = ImVec2(event.getX(), event.getY());
        return false;
    }

    bool imgui::Layer::onMouseScrolledEvent(event::MouseScrolledEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseWheel += event.getYOffset();
        io.MouseWheelH += event.getXOffset();
        return false;
    }

    // Keyboard events
    bool imgui::Layer::onKeyPressedEvent(event::KeyPressedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.AddKeyEvent(ImGuiKey_LeftCtrl, ImGui::IsKeyDown(ImGuiKey_LeftCtrl));
        io.AddKeyEvent(ImGuiKey_RightCtrl, ImGui::IsKeyDown(ImGuiKey_RightCtrl));
        io.AddKeyEvent(ImGuiKey_LeftShift, ImGui::IsKeyDown(ImGuiKey_LeftShift));
        io.AddKeyEvent(ImGuiKey_RightShift, ImGui::IsKeyDown(ImGuiKey_RightShift));
        return false;
    }

    bool imgui::Layer::onKeyReleasedEvent(event::KeyReleasedEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.AddKeyEvent(ImGuiKey_LeftCtrl, false);
        io.AddKeyEvent(ImGuiKey_RightCtrl, false);
        io.AddKeyEvent(ImGuiKey_LeftShift, false);
        io.AddKeyEvent(ImGuiKey_RightShift, false);
        return false;
    }

    bool imgui::Layer::onKeyTypedEvent(event::KeyTypedEvent &event)
    {
        return false;
    }

    // Window events
    bool imgui::Layer::onWindowResizeEvent(event::WindowResizeEvent &event)
    {
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(event.getWidth()), static_cast<float>(event.getHeight()));
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, event.getWidth(), event.getHeight());
        return false;
    }

    bool imgui::Layer::onWindowMovedEvent(event::WindowMovedEvent &event)
    {
        return false;
    }

    bool imgui::Layer::onWindowFocusEvent(event::WindowFocusEvent &event)
    {
        return false;
    }

    bool imgui::Layer::onWindowLostFocusEvent(event::WindowLostFocusEvent &event)
    {
        return false;
    }

    // App events
    bool imgui::Layer::onAppTickEvent(event::AppTickEvent &event)
    {
        return false;
    }

    bool imgui::Layer::onAppUpdateEvent(event::AppUpdateEvent &event)
    {
        return false;
    }

    bool imgui::Layer::onAppRenderEvent(event::AppRenderEvent &event)
    {
        return false;
    }
}