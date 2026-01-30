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

#include "utils/buffer.hpp"
#include "utils/shader.hpp"
#include "utils/vertex_array.hpp"

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
        std::shared_ptr<utils::Shader> m_Shader;
        std::shared_ptr<window::Window> m_Window;

        std::shared_ptr<utils::buffer::Vertex> m_VertexBuffer;
        std::shared_ptr<utils::buffer::Index> m_IndexBuffer;
        std::shared_ptr<utils::VertexArray> m_VertexArray;

        // square
        std::shared_ptr<utils::Shader> m_ShaderSquare;
        std::shared_ptr<utils::buffer::Index> m_IndexBufferSquare;
        std::shared_ptr<utils::VertexArray> m_SquareVertexArray;
        std::shared_ptr<utils::buffer::Vertex> m_VertexBufferSquare;

        imgui_layer::Layer *m_ImGuiLayer;

        bool m_Running = true;

        std::vector<std::function<void(event::Event &)>> m_EventListeners;

        bool onWindowClose(event::WindowCloseEvent &e);
        bool onWindowResize(event::WindowResizeEvent &e);

        layer::LayerStack m_LayerStack;

        static Application *s_Instance;
    };

    Application *createApplication();
}