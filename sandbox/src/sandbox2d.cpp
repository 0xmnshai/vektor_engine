#include "sandbox2d.hpp"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D")
{
    float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
    m_CameraController = std::make_shared<vektor::renderer::camera::Controller>(aspectRatio);
}

void Sandbox2D::onAttach()
{
    VEKTOR_CORE_INFO("Sandbox2D layer attached");
}

void Sandbox2D::onDetach()
{
    VEKTOR_CORE_INFO("Sandbox2D layer detached");
}

void Sandbox2D::onUpdate(vektor::core::Timestep timestep)
{
    VEKTOR_CORE_TRACE("Delta time: {0} seconds", timestep.getSeconds());

    m_CameraController->onUpdate(timestep);

    vektor::renderer::Command::setClearColor({0.13f, 0.13f, 0.13f, 1.0f}); // normalised / 255
    vektor::renderer::Command::clear();

    vektor::renderer::Renderer2D::beginScene(m_CameraController->getCamera());

    vektor::renderer::Renderer2D::drawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, m_Color);
    vektor::renderer::Renderer2D::drawQuad({0.5f, 0.5f}, {0.5f, 1.5f}, {0.8f, 0.2f, 0.3f, 1.0f});

    vektor::renderer::Renderer2D::endScene();
}

void Sandbox2D::onRender()
{
    ImGui::Begin("Color Settings");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_Color));
    ImGui::End();
}

void Sandbox2D::onEvent(vektor::event::Event &event)
{
    m_CameraController->onEvent(event);
}