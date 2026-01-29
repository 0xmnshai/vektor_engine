#pragma once

#include <vector>

#include "core/core.hpp"

#include "window/window.hpp"

#include "events/window.hpp"
#include "events/mouse.hpp"
#include "events/keyboard.hpp"

#include "layer/layer.hpp"
#include "layer/layer_stack.hpp"

#include "imgui/layer.hpp"

#include "utils/shader.hpp"

namespace vektor
{
    class VEKTOR_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
        void onEvent(event::Event &event);

        void addEventListener(const std::function<void(event::Event &)> &listener);

        void pushLayer(layer::Layer *layer);
        void popLayer(layer::Layer *layer);
        void pushOverlay(layer::Layer *overlay);
        void popOverlay(layer::Layer *overlay);

        inline window::Window &getWindow() { return *m_Window; }
        inline bool isRunning() { return m_Running; }

        inline window::Window &getWindow() const { return *m_Window; }
        inline static Application &getInstance() { return *s_Instance; }

    private:
        std::unique_ptr<window::Window> m_Window;
        // std::unique_ptr<imgui_layer::Layer> m_ImGuiLayer;
        imgui_layer::Layer *m_ImGuiLayer;

        unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
        std::unique_ptr<utils::Shader> m_Shader;

        bool m_Running = true;

        std::vector<std::function<void(event::Event &)>> m_EventListeners;

        bool onWindowClose(event::WindowCloseEvent &e);
        bool onWindowResize(event::WindowResizeEvent &e);

        layer::LayerStack m_LayerStack;

        static Application *s_Instance;
    };

    Application *CreateApplication();
}