#include "editor_layer.hpp"

glm::vec3 clearColor = glm::vec3(0.1f, 0.1f, 0.1f);

EditorLayer::EditorLayer()
    : Layer("Editor Layer")
{
    float aspectRatio = WINDOW_WIDTH / WINDOW_HEIGHT;
    m_CameraController = std::make_shared<vektor::renderer::camera::Controller>(aspectRatio);
}

void EditorLayer::onAttach()
{
    VEKTOR_CORE_INFO("EditorLayer layer attached");
}
void EditorLayer::onDetach()
{
    VEKTOR_CORE_INFO("EditorLayer layer detached");
}

void EditorLayer::onUpdate(vektor::core::Timestep timestep)
{
    VEKTOR_PROFILE_FUNCTION();

    VEKTOR_PROFILE_BEGIN_SESSION("EditorLayer::OnUpdate", "EditorLayer-Profile-Runtime.json");

    VEKTOR_CORE_TRACE("Delta time: {0} seconds", timestep.getSeconds());
    m_CameraController->onUpdate(timestep);

    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    VEKTOR_PROFILE_END_SESSION();
}

void EditorLayer::onRender()
{
    ImGui::Begin("Clear Color Settings");
    ImGui::ColorEdit3("Clear Color", glm::value_ptr(clearColor));
    ImGui::End();
}

void EditorLayer::onEvent(vektor::event::Event &event)
{
    m_CameraController->onEvent(event);
}