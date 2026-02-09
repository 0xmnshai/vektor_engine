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

    auto framebufferSpecification = vektor::renderer::FramebufferSpecification();
    framebufferSpecification.width = WINDOW_WIDTH;
    framebufferSpecification.height = WINDOW_HEIGHT;

    m_Framebuffer = vektor::renderer::Framebuffer::create(framebufferSpecification);

    m_ActiveScene = std::make_shared<vektor::scene::Scene>();

    // auto transformComponent = m_ActiveScene->createEntity<vektor::components::TransformComponent>();
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
    
    m_Framebuffer->bind();
    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_ActiveScene->onUpdate(timestep);

    vektor::renderer::Renderer2D::beginScene(m_CameraController->getCamera());

    // somthing ?

    vektor::renderer::Renderer2D::endScene();

    m_Framebuffer->unBind();

    VEKTOR_PROFILE_END_SESSION();
}

void EditorLayer::onRender()
{
    ImGui::Begin("Clear Color Settings");
    ImGui::ColorEdit3("Clear Color", glm::value_ptr(clearColor));
    ImGui::End();

    // using frame buffer here 

    ImGui::Begin("Frame buffer viewport");
    auto viewportSize = ImGui::GetContentRegionAvail();
    m_Framebuffer->resize(viewportSize.x, viewportSize.y);
    ImGui::Image((void*)(intptr_t)m_Framebuffer->getColorAttachmentRendererID(), ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
}

void EditorLayer::onEvent(vektor::event::Event &event)
{
    m_CameraController->onEvent(event);
}