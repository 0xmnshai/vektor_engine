
#include "vektor.hpp"

#include "events/event.hpp"
#include "layer/layer.hpp"
#include "entry_point.hpp"

#include "imgui/layer.hpp"

class ExampleLayer : public vektor::layer::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void onUpdate() override
    {
        VEKTOR_INFO("ExampleLayer::Update");
    }

    void onEvent(vektor::event::Event &event) override
    {
        VEKTOR_TRACE("{0}", event.toString());
    }
};

class Sandbox : public vektor::Application
{
public:
    Sandbox()
    {
        std::cout << "Sandbox::Sandbox" << std::endl;

        pushLayer(new ExampleLayer());
        pushLayer(new vektor::imgui_layer::Layer());
    }

    ~Sandbox()
    {
    }
};

vektor::Application *vektor::CreateApplication()
{
    return new Sandbox();
}