
#include "vektor.hpp"

#include "events/event.hpp"
#include "layer/layer.hpp"
#include "entry_point.hpp"

#include "imgui/layer.hpp"

#include "input/input.hpp"
#include "input/window.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 camera(float translate, glm::vec2 const &rotate)
{
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -translate));
    view = glm::rotate(view, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    return projection * view;
}

class ExampleLayer : public vektor::layer::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
        glm::mat4 cam = camera(5.0f, glm::vec2(0.0f, 0.0f));
    }

    void onUpdate() override
    {
        if (vektor::input::Input::isKeyPressed(VEKTOR_KEY_DELETE))
        {
            // VEKTOR_INFO("Delete key pressed");
        }
    }

    void onEvent(vektor::event::Event &event) override
    {
        // VEKTOR_TRACE("{0}", event.toString());
        if (event.getEventType() == vektor::event::EventType::KeyPressed)
        {
            vektor::event::KeyPressedEvent &kp = (vektor::event::KeyPressedEvent &)event;
            VEKTOR_CORE_TRACE("Key pressed character: {0}", (char)kp.getKeyCode());
        }
    }
};

class Sandbox : public vektor::Application
{
public:
    Sandbox()
    {
        std::cout << "Sandbox::Sandbox" << std::endl;

        // pushLayer(new ExampleLayer());
        // pushLayer(new vektor::imgui_layer::Layer());
    }

    ~Sandbox()
    {
    }
};

vektor::Application *vektor::createApplication()
{
    return new Sandbox();
}