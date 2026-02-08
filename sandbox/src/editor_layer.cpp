#include "sandbox.hpp"

Sandbox::Sandbox()
    : Layer("Sandbox")
{
    float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    m_CameraController = std::make_shared<vektor::renderer::camera::Controller>(aspectRatio);
    m_CameraController->setZoomLevel(5.0f);
}

Sandbox::~Sandbox()
{
}

void Sandbox::onAttach()
{
}
void Sandbox::onDetach()
{
}
void Sandbox::onUpdate(vektor::core::Timestep timestep)
{
    m_CameraController->onUpdate(timestep);

    vektor::renderer::Command::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    vektor::renderer::Command::clear();

    vektor::renderer::Renderer2D::beginScene(m_CameraController->getCamera());

    // render

    vektor::renderer::Renderer2D::endScene();
}
void Sandbox::onRender()
{
}
void Sandbox::onEvent(vektor::event::Event &event)
{
}