#include "editor_layer.hpp"

EditorLayer::EditorLayer()
    : Layer("EditorLayer")
{
    float aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    m_CameraController = std::make_shared<vektor::renderer::camera::Controller>(aspectRatio);
    m_CameraController->setZoomLevel(5.0f);
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::onAttach()
{
}
void EditorLayer::onDetach()
{
}
void EditorLayer::onUpdate(vektor::core::Timestep timestep)
{
    m_CameraController->onUpdate(timestep);

    vektor::renderer::Command::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    vektor::renderer::Command::clear();

    vektor::renderer::Renderer2D::beginScene(m_CameraController->getCamera());

    // render

    vektor::renderer::Renderer2D::endScene();
}
void EditorLayer::onRender()
{
}
void EditorLayer::onEvent(vektor::event::Event &event)
{
}